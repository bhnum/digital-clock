#pragma once

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>

// 4-digit 7-segment driver
// mappings:
// D0 to D6			-> 7-seg a to d
// D7				-> 7-seg dp
// B4, B2, B1, B0	-> 7-seg digits 1 to 4
#define SEGMENT_PORT	PORTD
#define DIGIT_PORT		PORTB
#define PDIGIT1			PB4
#define PDIGIT2			PB2
#define PDIGIT3			PB1
#define PDIGIT4			PB0

class SevenSegment
{
public:
	void Initialize()
	{
		// TODO
	}
	
	void TurnOff()
	{
		// TODO
	}
	
	void TurnOn()
	{
		// TODO
	}
	
	void Next()
	{
		// TODO
	}
	
	void SetData(const uint8_t digits[4], bool show_dots)
	{
		cli();
		for (uint8_t i = 0; i < 4; i++)
			this->digits[i] = digits[i];
		this -> show_dots = show_dots;
		sei();
	}
	
private:
	static const uint8_t segments[] PROGMEM;

	uint8_t digits[4];
	bool show_dots = false;
};

extern SevenSegment seven_segment;
