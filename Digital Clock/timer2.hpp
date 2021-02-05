#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

// timer2 drives the buzzer on pin PB3 (OC2A)
class Timer2
{
public:	
	void Initialize()
	{
		OC2A_PORT |= 1 << OC2A_BIT;
		OC2A_DDR |= 1 << OC2A_BIT;
	}
	
	void TurnOff()
	{
		TCCR2A = 0;
		TCCR2B = 0;
	}
	
	void TurnOn()
	{
		// Timer 2 on phase correct PWM, output on OC2A, duty cycle 50%
		TCCR2A = (1 << COM2A1) | (1 << COM2A0) | (0 << COM2B1) | (0 << COM2B0) | (0 << WGM21) | (1 << WGM20);
		TCCR2B = (0 << WGM22) | (0 << CS22) | (1 << CS21) | (0 << CS20);
		OCR2A = 0x80;
	}
	
	// TODO: set frequency
	//void SetFrequency(uint16_t frequency)
	//{
		//ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		//{
			//this->frequency = frequency;
		//}
	//}
	
private:
	//uint16_t frequency = 200;
};

extern Timer2 timer2;
