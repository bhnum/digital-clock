#include "timer0.hpp"

Timer0 timer0 = Timer0();

ISR(TIMER0_COMPA_vect)
{
	seven_segment.TurnOff();
	seven_segment.Next();
}

ISR(TIMER0_COMPB_vect)
{
	static uint8_t n = 0;
	n++;
	if (n % 8 == 0) // process key press in 2ms
		keypad.Process();
}

ISR(TIMER0_OVF_vect)
{
	seven_segment.TurnOn();
}
