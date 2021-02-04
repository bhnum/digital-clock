#pragma once

#include <avr/interrupt.h>
#include <avr/io.h>

enum class Key : uint8_t
{
	None = 0,
	Up, Down, Left, Right, Ok 
};

// 5-button keypad including up, down, left, right, enter
// mappings:
// C0	-> down, up
// C1	-> left, right
// C2	-> Ok
class Keypad
{
public:
	void Initialize()
	{
		// TODO
	}
	
	// called from ISR
	void Process()
	{
		// TODO
	}
	
	Key GetKey()
	{
		cli();
		// TODO
		return Key::None;
		sei();
	}
	
private:
	uint8_t buffer[3] = { 0 };
};

extern Keypad keypad;
