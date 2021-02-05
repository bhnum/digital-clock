#pragma once

#include <avr/pgmspace.h>
#include "utility.hpp"

struct DateTime
{
public:
	static const char DayName[7][10] PROGMEM;

	int8_t year = 0; // starts from year 2000
	int8_t month = 1;
	int8_t day = 1; // day of month
	int8_t day_of_week = 1; // day of week
	
	int8_t hour = 0;
	int8_t minute = 0;
	int8_t second = 0;
	
	void normalize()
	{
		minute += posdiv(second, 60);
		second = modulo(second, 60);
		
		hour += posdiv(minute, 60);
		minute = modulo(minute, 60);
		
		day += posdiv(hour, 24);
		day_of_week += posdiv(hour, 24);
		hour = modulo(hour, 24);
		
		month += posdiv(day - 1, month_days(month, year));
		day = modulo(day - 1, month_days(month, year)) + 1;
		
		year += posdiv(month - 1, 12);
		month = modulo(month - 1, 12) + 1;
		
		year = modulo(year, 100);
		
		day_of_week = modulo(day_of_week - 1, 7) + 1;
	}
	
	bool operator ==(const DateTime& other) const
	{
		return this->year == other.year
			&& this->month == other.month
			&& this->day == other.day
			&& this->hour == other.hour
			&& this->minute == other.minute
			&& this->second == other.second;
	}
	
	bool operator !=(const DateTime& other) const
	{
		return !(*this == other);
	}
	
	bool operator >(const DateTime& other) const
	{
		if (this->year > other.year)
			return true;
		if (this->year < other.year)
			return false;
			
		if (this->month > other.month)
			return true;
		if (this->month < other.month)
			return false;
			
		if (this->day > other.day)
			return true;
		if (this->day < other.day)
			return false;
			
		if (this->hour > other.hour)
			return true;
		if (this->hour < other.hour)
			return false;
			
		if (this->minute > other.minute)
			return true;
		if (this->minute < other.minute)
			return false;
			
		if (this->second > other.second)
			return true;
		if (this->second < other.second)
			return false;

		return false;
	}
	
	bool operator <(const DateTime& other) const
	{
		return other > *this;
	}
	
	bool operator >=(const DateTime& other) const
	{
		return *this > other || *this == other;
	}
	
	bool operator <=(const DateTime& other) const
	{
		return *this < other || *this == other;
	}
	
private:
	uint8_t month_days(uint8_t month, uint8_t year)
	{
		if (month == 2) // Feb
		{
			// leap years: year % 4 == 0, year 2000 is a leap year
			if (year % 4 == 0)
				return 29;
			return 28;
		}
		else if (month % 2 == 1)
			return 31;
		else
			return 30;
	}
};
