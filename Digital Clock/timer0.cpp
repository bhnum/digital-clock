#include "timer0.hpp"
#include "beeper.hpp"

Timer0 timer0 = Timer0();

ISR(TIMER0_COMPA_vect)
{
#ifndef	SIMULATION
	seven_segment.TurnOff();
	seven_segment.Next();
#endif
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
#ifndef	SIMULATION
	seven_segment.TurnOn();
#else
	static uint8_t n = 0;
	n++;
	if (n % 16 == 0)
	{
		seven_segment.TurnOff();
		seven_segment.Next();
		seven_segment.TurnOn();
	}
#endif
	beeper.Process();
}
