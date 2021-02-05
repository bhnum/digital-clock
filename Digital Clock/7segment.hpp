#pragma once

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include <stdint.h>

// 4-digit 7-segment driver
// mappings:
// D0 to D6			-> 7-seg a to d
// D7				-> 7-seg dp
// B4, B2, B1, B0	-> 7-seg digits 1 to 4
#define SEGMENT_PORT	PORTD
#define SEGMENT_DDR		DDRD
#define DIGIT_PORT		PORTB
#define DIGIT_DDR		DDRB
#define PDIGIT1			PB4
#define PDIGIT2			PB2
#define PDIGIT3			PB1
#define PDIGIT4			PB0

class SevenSegment
{
public:
	void Initialize()
	{
		SEGMENT_DDR = 0xff;
		DIGIT_DDR |= (1 << PDIGIT1) | (1 << PDIGIT2) | (1 << PDIGIT3) | (1 << PDIGIT4);
	}
	
	void TurnOff()
	{
		DIGIT_PORT &= ~((1 << PDIGIT1) | (1 << PDIGIT2) | (1 << PDIGIT3) | (1 << PDIGIT4));
	}
	
	void TurnOn()
	{
		uint8_t out = pgm_read_byte(&segments[digits[index]]);
		if (show_dots)
			out |= 0x80;
#ifdef SIMULATION
		SEGMENT_PORT = ~out;
#else
		SEGMENT_PORT = out;
#endif
		
		switch (index)
		{
		case 0:
			DIGIT_PORT = (1 << PDIGIT1);
			break;
		case 1:
			DIGIT_PORT = (1 << PDIGIT2);
			break;
		case 2:
			DIGIT_PORT = (1 << PDIGIT3);
			break;
		case 3:
			DIGIT_PORT = (1 << PDIGIT4);
			break;
		}
	}
	
	void Next()
	{
		index = (index + 1) % 4;
	}
	
	void SetData(const uint8_t digits[4], bool show_dots)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			for (uint8_t i = 0; i < 4; i++)
			this->digits[i] = digits[i];
			this->show_dots = show_dots;
		}
	}	

	void SetData(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, bool show_dots)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			digits[0] = d1;
			digits[1] = d2;
			digits[2] = d3;
			digits[3] = d4;
			this->show_dots = show_dots;
		}
	}
	
private:
	static const uint8_t segments[] PROGMEM;

	uint8_t index = 0;
	uint8_t digits[4]; // little-endian
	bool show_dots = false;
};

extern SevenSegment seven_segment;
