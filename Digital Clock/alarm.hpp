#pragma once

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/atomic.h>
#include "utility.hpp"
#include "datetime.hpp"

enum class AlarmOption : uint8_t
{
	Once = 0, EveryDay
};

struct alignas(4) Alarm
{
	DateTime time;
	AlarmOption option;
	
	// TODO
	//char message[16];
} __attribute__((packed));

// First 128 bytes of EEPROM is allocated for alarms => max number of alarms = 15
class AlarmManager
{
public:
	void Initialize()
	{
		ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
		{
			uint8_t version = eeprom_read_byte(version_ptr);
			
			if (version != current_version)
			{
				// EEPROM content is invalid
				eeprom_update_byte(number_ptr, 0);
				eeprom_update_byte(version_ptr, current_version);
				number = 0;
			}
			else
				number = eeprom_read_byte(number_ptr);
		}
	}
	
	void AddAlarm(const Alarm& alarm)
	{
		ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
		{
			eeprom_update_block(&alarm, GetPointer(number), sizeof(Alarm));
			number++;
			eeprom_update_byte(number_ptr, number);
		}
	}
	
	void GetAlarm(size_t index, Alarm& alarm)
	{
		ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
		{
			eeprom_read_block(&alarm, GetPointer(index), sizeof(Alarm));
		}
	}
	
	void UpdateAlarm(size_t index, const Alarm& alarm)
	{
		ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
		{
			eeprom_update_block(&alarm, GetPointer(index), sizeof(Alarm));
		}
	}
	
	void RemoveAlarm(size_t index)
	{
		ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
		{
			for (size_t i = index; i < number - 1; i++)
			{
				Alarm alarm;
				eeprom_read_block(&alarm, GetPointer(i + 1), sizeof(Alarm));
				eeprom_update_block(&alarm, GetPointer(i), sizeof(Alarm));
			}
			number--;
			eeprom_update_byte(number_ptr, number);
		}
	}
	
	void ClearAll()
	{
		number = 0;
		ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
		{
			eeprom_update_byte(number_ptr, 0);
		}
	}
	
	size_t GetCount()
	{
		return number;
	}
	
private:
	void* GetPointer(size_t index)
	{
		return (void*)((index + 1) * sizeof(Alarm));
	}
	
	size_t number;
	
	uint8_t* const number_ptr = (uint8_t*)0;
	uint8_t* const version_ptr = (uint8_t*)1;
};

extern AlarmManager alarm_manager;
