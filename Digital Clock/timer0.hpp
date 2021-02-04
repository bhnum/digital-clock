#pragma once

#include <avr/interrupt.h>
#include "7segment.hpp"
#include "keypad.hpp"

class Timer0
{
public:	
	void Initialize()
	{
		// TODO: put timer0 on CTC
		// on compare match turn off 7 seg, change digit
		// on overflow turn on 7 seg, prcess key press
		// enable interrupts, start timer0
	}
	
	// 0 <= proportion < 256
	void SetDutyCycle(uint8_t proportion)
	{
		// TODO: set OCRA
	}
};

extern Timer0 timer0;
