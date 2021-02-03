#include <avr/io.h>
#include <util/delay.h>

#include "text.hpp"

int main(void)
{
	PORTC = 0xff;
	
	i2c.Initialize();
	
	_delay_ms(200);
	
	uint8_t img[3][3] = {{0xaa, 0x55, 0xaa}, {8, 0, 8}, {0xaa, 0x55, 0xaa}};
	
	display.Initialize(true);
	display.Draw((uint8_t*)img, 3, 3);
	display.Draw((uint8_t*)img, 3, 3);
	
	text.PrintProgMem(PSTR("Hello!\nTesting\t1234\b\bab"));
	
	text.GoToXY(3, 5);
	text.PrintProgMem(PSTR("Hello!\nTesting\t1234\b\bab"));
	

	// set PORTC1 to output
	DDRC |= 1 << DDC2;

	/* Replace with your application code */
	while (1)
	{
		_delay_ms(1000);
		PORTC ^= 1 << PORTC2;
	}
}

