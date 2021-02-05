#pragma once

#include "timer2.hpp"

// This class only manages timer 2
// TODO: add frequency
class Beeper
{
public:
	// This function should be called every 0.256ms ~ 1/4ms
	void Process()
	{
		if (!on)
			return;
			
		if (n < beep_times * on_off_period)
		{
			if (n % on_off_period == on_time)
				timer2.TurnOn();
			else if (n % on_off_period == off_time)
				timer2.TurnOff();
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
		timer2.TurnOff();
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
};

extern Beeper beeper;
