#pragma once

#include "i2c.hpp"
#include "datetime.hpp"

// DS1307 RTC driver, uses I2C for communication
class RTC
{
public:
	static const uint8_t i2c_address = 0b1101000;

	void GetTime(DateTime& time)
	{
		uint8_t seconds;
		uint8_t minutes;
		uint8_t hours;
		uint8_t day_of_week;
		uint8_t day;
		uint8_t month;
		uint8_t year;
		
		i2c.Start();
		i2c.WriteAddress(i2c_address, true);
		i2c.Write(0);
		i2c.Start();
		i2c.WriteAddress(i2c_address, false);
		i2c.Read(seconds, true);
		i2c.Read(minutes, true);
		i2c.Read(hours, true);
		i2c.Read(day_of_week, true);
		i2c.Read(day, true);
		i2c.Read(month, true);
		i2c.Read(year, false);
		i2c.Stop();
		
		time.second = from_bcd(seconds & 0x7f);
		time.minute = from_bcd(minutes);
		time.hour = from_bcd(hours & 0x3f);
		time.day_of_week = day_of_week;
		time.day = from_bcd(day);
		time.month = from_bcd(month);
		time.year = from_bcd(year);
	}
	
	void SetTime(const DateTime& time)
	{
		i2c.Start();
		i2c.WriteAddress(i2c_address, true);
		i2c.Write(0);
		i2c.Write(to_bcd(time.second));
		i2c.Write(to_bcd(time.minute));
		i2c.Write(to_bcd(time.hour));
		i2c.Write(to_bcd(time.day_of_week));
		i2c.Write(to_bcd(time.day));
		i2c.Write(to_bcd(time.month));
		i2c.Write(to_bcd(time.year));
		i2c.Write(0);
		i2c.Stop();
	}
};

extern RTC rtc;
