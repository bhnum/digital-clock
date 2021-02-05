#include <avr/io.h>
#include <util/delay.h>

#include "text.hpp"
#include "keypad.hpp"
#include "timer0.hpp"
#include "timer2.hpp"
#include "beeper.hpp"
#include "app.hpp"
#include "alarm.hpp"

int main(void)
{
	_delay_ms(1000);
	
	i2c.Initialize();
	keypad.Initialize();
	seven_segment.Initialize();
	timer0.Initialize();
	timer2.Initialize();
	display.Initialize(true);
	alarm_manager.Initialize();
	
	DateTime time, last_time;
	App app = App();
	
	sei();
	
	rtc.GetTime(time);
	app.Run(Event::Start, Key::None, time);
	
	while (1)
	{
		rtc.GetTime(time);
		if (time != last_time)
		{
			last_time = time;
			seven_segment.SetData(time.hour / 10, time.hour % 10, time.minute / 10, time.minute % 10, time.second % 2 == 0);
			
			app.Run(Event::TimeChange, Key::None, time);
		}
		
		Key key = keypad.GetKeyPress();
		if (key != Key::None)
		{
			app.Run(Event::KeyPress, key, time);
		}
		else
			_delay_ms(100);
	}
}

