#pragma once

#include "i2c.hpp"
#include "datetime.hpp"

// DS1307 RTC driver, uses I2C for communication
class RTC
{
public:
	void Initialize()
	{
		// TODO
	}
	
	void GetTime(DateTime& time)
	{
		// TODO
	}
	
	void SetTime(const DateTime& time)
	{
		// TODO
	}
};

extern RTC rtc;
