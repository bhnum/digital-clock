#include <avr/io.h>
#include <util/delay.h>

#include "text.hpp"
#include "keypad.hpp"
#include "timer0.hpp"

int main(void)
{
	i2c.Initialize();
	keypad.Initialize();
	seven_segment.Initialize();
	timer0.Initialize();
	
	uint8_t img[3][3] = {{0xaa, 0x55, 0xaa}, {8, 0, 8}, {0xaa, 0x55, 0xaa}};
	
	display.Initialize(true);
	display.Draw((uint8_t*)img, 3, 3);
	display.Draw((uint8_t*)img, 3, 3);
	
	text.PrintProgMem(PSTR("Hello!\nTesting\t\a1\a234\b\bab"), true);
	
	text.GoToXY(3, 5);
	text.Print("Hello!\nTesting\t1234\b\aab\a");
	
	text.GoToXY(6, 0);
	
// 	KEYPAD_PORT |= (1 << KEYPAD_LEFT_RIGHT) | (1 << KEYPAD_UP_DOWN);
// 	_delay_ms(2000);
// 	KEYPAD_PORT &= ~((1 << KEYPAD_LEFT_RIGHT) | (1 << KEYPAD_UP_DOWN));

	/* Replace with your application code */
	while (1)
	{
		_delay_ms(200);
		//keypad.Process();
		Key key = keypad.GetKeyPress();
		if (key != Key::None)
		text.Print('0' + (uint8_t)key);
	}
}

