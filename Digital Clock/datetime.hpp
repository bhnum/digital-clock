#pragma once

#include "utility.hpp"

struct DateTime
{
public:
	int8_t year; // starts from year 2000
	int8_t month;
	int8_t day; // day of month
	int8_t day_of_week; // day of week
	
	int8_t hour;
	int8_t minute;
	int8_t second;
	
	void normalize()
	{
		minute += posdiv(second, 60);
		second = modulo(second, 60);
		
		hour += posdiv(minute, 60);
		minute = modulo(minute, 60);
		
		day += posdiv(hour, 60);
		hour = modulo(hour, 60);
		
		month += posdiv(day - 1, month_days(month, year));
		day = modulo(day - 1, month_days(month, year)) + 1;
		
		year += posdiv(month - 1, 12);
		month = modulo(month - 1, 12) + 1;
		
		year = modulo(year, 100);
	}
	
private:
	uint8_t month_days(uint8_t month, uint8_t year)
	{
		if (month == 2) // Feb
		{
			// leap years: year % 4 == 0, except 2000 is not a leap year
			if (year != 0 && year % 4 == 0)
				return 29;
			return 28;
		}
		else if (month % 2 == 1)
			return 31;
		else
			return 30;
	}
};
