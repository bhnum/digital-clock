#pragma once

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/atomic.h>
#include "utility.hpp"

// This class manages timer 1
class Beeper
{
public:
	void Initialize()
	{
		LoadData();
	}

	// This function should be called every 0.256ms ~ 1/4ms
	void Process()
	{
		if (!on)
			return;
			
		if (n < beep_times * on_off_period)
		{
			if (n % on_off_period == on_time)
				StartBeep();
			else if (n % on_off_period == off_time)
				StopBeep();
		}
		
		n++;
		if (n >= period)
			n = 0;
	}

	void TurnOn()
	{
		n = 0;
		on = true;
	}
	
	void TurnOff()
	{
		on = false;
		StopBeep();
	}
	
	void StartBeep()
	{
		OC1B_DDR |= 1 << OC1B_BIT;
		
		// Timer 1 phase and frequency correct PWM, output OC1B pin, duty cycle depending on loudness
		TCNT1 = 0;
		ICR1 = F_CPU / 2 / frequency;
		OCR1B = F_CPU / 4 / frequency * loudness / 100;
		TCCR1A = (1 << COM1B1) | (1 << COM1B0) | (0 << WGM11) | (0 << WGM10);
		TCCR1B = (1 << WGM13) | (0 << WGM12) | (0 << CS12) | (0 << CS11) | (1 << CS10);
	}
	
	void StopBeep()
	{
		TCCR1A = 0;
		TCCR1B = 0;
		OC1B_DDR &= ~(1 << OC1B_BIT);
	}
	
	uint8_t GetLoudness() { return loudness; }
	uint16_t GetFrequency() { return frequency; }
	
	void SetLoudness(uint8_t loudness)
	{
		this->loudness = loudness;
		OCR1B = F_CPU / 4 / frequency * loudness / 100;
	}
	
	void SetFrequency(uint16_t frequency)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			this->frequency = frequency;
		}
		ICR1 = F_CPU / 2 / frequency;
		OCR1B = F_CPU / 4 / frequency * loudness / 100;
	}
	
	void SaveData()
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			EEPROMData data = { version: current_version, loudness: loudness, frequency: frequency };
			eeprom_update_block(&data, eeprom_data_ptr, sizeof(EEPROMData));
		}
	}
	
	void LoadData()
	{
		EEPROMData data;
		ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
		{
			eeprom_read_block(&data, eeprom_data_ptr, sizeof(EEPROMData));
			
			if (data.version != current_version)
			{
				// EEPROM content is invalid
				data.version = current_version;
				data.loudness = 100;
				data.frequency = 2000;
				eeprom_update_block(&data, eeprom_data_ptr, sizeof(EEPROMData));
			}
		}
		loudness = data.loudness;
		frequency = data.frequency;
	}
	
private:
	// timings are in 1/4ms unit
	static const uint16_t period = 1000 * 4;
	static const uint16_t on_time = 0 * 4;
	static const uint16_t off_time = 80 * 4;
	static const uint16_t on_off_period = 160 * 4;
	static const uint8_t beep_times = 4;

	uint16_t n = 0;
	bool on = false;
	
	uint8_t loudness; // in percents
	uint16_t frequency;
	
	struct EEPROMData
	{
		uint8_t version;
		uint8_t loudness;
		uint16_t frequency;
	} __attribute__((packed));
	
	EEPROMData* const eeprom_data_ptr = (EEPROMData*)0x80;
};

extern Beeper beeper;
