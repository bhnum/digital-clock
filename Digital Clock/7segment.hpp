#pragma once

// 4-digit 7-segment driver on port D
// mappings:
// D0 to D6	-> 7-seg a to d
// D7		-> 7-seg dp
// B3 to B0 -> 7-seg digits 1 to 4
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
	
	void TurnOff()
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
	static const segment_port = PORTD;
	static const digit_port = PORTB;

	uint8_t digits[4]
	bool show_dots = false;
};

extern SevenSegment seven_segment;
