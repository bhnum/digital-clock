#pragma once

#include <stddef.h>
#include <avr/eeprom.h>
#include <util/atomic.h>
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
};

class AlarmManager
{
public:
	void Initialize()
	{
		ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
		{
			number = eeprom_read_byte(0);
			if (number == 0xff) // uninitialized
			{
				eeprom_update_byte(0, 0);
				number = 0;
			}
		}
	}
	
	void AddAlarm(const Alarm& alarm)
	{
		ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
		{
			eeprom_update_block(&alarm, GetPointer(number), sizeof(Alarm));
			number++;
			eeprom_update_byte(0, number);
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
			eeprom_update_byte(0, number);
		}
	}
	
	void ClearAll()
	{
		number = 0;
		ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
		{
			eeprom_update_byte(0, 0);
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
};

extern AlarmManager alarm_manager;
