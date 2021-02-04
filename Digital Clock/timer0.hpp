#pragma once

#include <avr/interrupt.h>
#include "7segment.hpp"
#include "keypad.hpp"

class Timer0
{
public:	
	void Initialize()
	{
		// Timer 0 period 0.256 ms ~ 4kHz for MCU clock 8MHz
		TCCR0B = (0 << CS02) | (1 << CS01) | (0 << CS00);
		
		TCNT0 = 0x00;
		OCR0A = 0x80;
		OCR0B = 0xB0;
		TIMSK0 = (1 << TOIE0) | (1 << OCIE0A) | (1 << OCIE0B);
	}
	
	// 0 <= proportion < 256
	void SetDutyCycle(uint8_t proportion)
	{
		OCR0A = proportion;
		if (proportion > 0x80)
			OCR0B = 0x40;
		else
			OCR0B = 0xB0;
	}
};

extern Timer0 timer0;
