#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

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
#define KEYPAD_PORT			PORTC
#define KEYPAD_DDR			DDRC
#define KEYPAD_PIN			PINC
#define KEYPAD_UP_DOWN		PC0
#define KEYPAD_LEFT_RIGHT	PC1
#define KEYPAD_OK			PC2

class Keypad
{
public:
	void Initialize()
	{
		KEYPAD_PORT |= (1 << KEYPAD_OK);
	}
	
	// called from ISR
	void Process()
	{
		if (pulled_up)
		{
			buffer_down <<= 1;
			buffer_down |= ((~KEYPAD_PIN) >> KEYPAD_UP_DOWN) & 1;
			buffer_left <<= 1;
			buffer_left |= ((~KEYPAD_PIN) >> KEYPAD_LEFT_RIGHT) & 1;
			
			KEYPAD_PORT &= ~((1 << KEYPAD_LEFT_RIGHT) | (1 << KEYPAD_UP_DOWN));
		}
		else
		{
			buffer_up <<= 1;
			buffer_up |= (KEYPAD_PIN >> KEYPAD_UP_DOWN) & 1;
			buffer_right <<= 1;
			buffer_right |= (KEYPAD_PIN >> KEYPAD_LEFT_RIGHT) & 1;
			
			KEYPAD_PORT |= (1 << KEYPAD_LEFT_RIGHT) | (1 << KEYPAD_UP_DOWN);
		}
		buffer_ok <<= 1;
		buffer_ok |= (~KEYPAD_PIN >> KEYPAD_OK) & 1;
		pulled_up = !pulled_up;
	}
	
	Key GetKeyPress()
	{
		Key key = GetCurrentKey();
		if (key == last_key)
			return Key::None;
		last_key = key;
		return key;
	}
	
	Key GetCurrentKey()
	{
		Key key = Key::None;
		
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			if (buffer_up == 0xff)
				key = Key::Up;
			if (buffer_down == 0xff)
				key = Key::Down;
			if (buffer_left == 0xff)
				key = Key::Left;
			if (buffer_right == 0xff)
				key = Key::Right;
			if (buffer_ok == 0xff)
				key = Key::Ok;
		}
		
		return key;
	}
	
private:
	uint8_t buffer_up = 0;
	uint8_t buffer_down = 0;
	uint8_t buffer_left = 0;
	uint8_t buffer_right = 0;
	uint8_t buffer_ok = 0;
	
	Key last_key = Key::None;
	bool pulled_up = false;
};

extern Keypad keypad;
