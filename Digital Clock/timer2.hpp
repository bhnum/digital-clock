#pragma once

#include <avr/interrupt.h>

// timer2 drives the buzzer on pin PB3 (OC2A)
class Timer2
{
public:	
	void Initialize()
	{
		// TODO?
	}
	
	void TurnOff()
	{
		// TODO
	}
	
	void TurnOn()
	{
		// TODO: put timer2 on phase correct pwm
		// frequency set as class field "frequency"
		// output on OC2A
		// duty cycle 50%
	}
	
	void SetFrequency(uint16_t frequency)
	{
		cli();
		this->frequency = frequency;
		// TODO
		sei();
	}
	
private:
	uint16_t frequency = 200;
};

extern Timer2 timer2;
