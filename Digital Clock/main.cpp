#include <avr/io.h>
#include <util/delay.h>

#include "display.hpp"


int main(void)
{
	PORTC = 0xff;
	
	i2c.Initialize();
	
	_delay_ms(200);
	
	display.Initialize(true);
	display.StartData();
	display.Send(0xaa);
	display.Send(0x55);
	display.Send(0xaa);
	display.Send(0x55);
	display.Send(0xaa);
	display.Send(0x55);
	display.Send(0xaa);
	display.Send(0x55);
	display.Stop();

	// set PORTC1 to output
	DDRC |= 1 << DDC2;

	/* Replace with your application code */
	while (1)
	{
		_delay_ms(1000);
		PORTC ^= 1 << PORTC2;
	}
}

