#include "app.hpp"
#include <stdlib.h>
#include <util/delay.h>

void App::Run(Event event, Key key, const DateTime& time)
{
	switch (current_page)
	{
	case Page::Void:
		PageVoid(event, key, time);
		break;
	case Page::MainMenu:
		PageMainMenu(event, key, time);
		break;
	case Page::SetTime:
		PageSetTime(event, key, time);
		break;
	}
	
	if (last_page != current_page)
	{
		last_page = current_page;
		Run(Event::Start, Key::None, time);
	}
}

void App::PageVoid(Event event, Key key, const DateTime& time)
{
	if (event == Event::Start)
	{
		display.ClearScreen();
		display.TurnOff();
	}
		
	if (event == Event::KeyPress)
	{
		current_page = Page::MainMenu;
		display.TurnOn();
	}
}

void App::PageMainMenu(Event event, Key key, const DateTime& time)
{
	static int8_t index = 0;
	
	if (event == Event::Start)
	{
		index = 0;
	}
	
	if (event == Event::Start || event == Event::TimeChange)
	{
		display.ClearScreen();
		text.PrintProgMem(PSTR(" Set Time\n"));
		text.PrintProgMem(PSTR(" Turn the Screen Off\n"));
		
		text.GoToXY(6, 4);
		text.Print('0' + time.hour / 10);
		text.Print('0' + time.hour % 10);
		text.Print(':');
		text.Print('0' + time.minute / 10);
		text.Print('0' + time.minute % 10);
		text.Print(':');
		text.Print('0' + time.second / 10);
		text.Print('0' + time.second % 10);
		
		text.GoToXY(5, 6);
		text.PrintProgMem(PSTR("20"));
		text.Print('0' + time.year / 10);
		text.Print('0' + time.year % 10);
		text.Print('-');
		text.Print('0' + time.month / 10);
		text.Print('0' + time.month % 10);
		text.Print('-');
		text.Print('0' + time.day / 10);
		text.Print('0' + time.day % 10);
	}
	
	if (event == Event::KeyPress)
	{
		text.GoToXY(0, index);
		text.Print(' ');
		if (key == Key::Ok)
		{
			if (index == 0)
				current_page = Page::SetTime;
			else if (index == 1)
				current_page = Page::Void;
		}
		else if (key == Key::Down)
			index = min(1, index + 1);
		else if (key == Key::Up)
			index = max(0, index - 1);
	}
	text.GoToXY(0, index);
	text.Print('>');
}

void App::PageSetTime(Event event, Key key, const DateTime& time)
{
	static int8_t index = 1;
	
	if (event == Event::Start)
	{
		index = 1;
	}
	
	if (event == Event::Start || event == Event::TimeChange)
	{
		display.ClearScreen();
		text.PrintProgMem(PSTR("Press Ok to return"));
		
		text.GoToXY(6, 3);
		text.Print('0' + time.hour / 10);
		text.Print('0' + time.hour % 10);
		text.Print(':');
		text.Print('0' + time.minute / 10);
		text.Print('0' + time.minute % 10);
		text.Print(':');
		text.Print('0' + time.second / 10);
		text.Print('0' + time.second % 10);
		
		text.GoToXY(5, 5);
		text.PrintProgMem(PSTR("20"));
		text.Print('0' + time.year / 10);
		text.Print('0' + time.year % 10);
		text.Print('-');
		text.Print('0' + time.month / 10);
		text.Print('0' + time.month % 10);
		text.Print('-');
		text.Print('0' + time.day / 10);
		text.Print('0' + time.day % 10);
	}
	
	if (event == Event::KeyPress)
	{
		if (index == 1 || index == 2)
			text.GoToXY(6 + index - 1, 4);
		else if (index == 3 || index == 4)
			text.GoToXY(6 + 1 + index - 1, 4);
		else if (index == 5 || index == 6)
			text.GoToXY(6 + 2 + index - 1, 4);
		else if (index == 7 || index == 8)
			text.GoToXY(7 + index - 7, 6);
		else if (index == 9 || index == 10)
			text.GoToXY(7 + 1 + index - 7, 6);
		else if (index == 11 || index == 12)
			text.GoToXY(7 + 2 + index - 7, 6);
		text.Print(' ');
		
		if (key == Key::Ok)
			current_page = Page::MainMenu;
		else if (key == Key::Right)
			index = min(12, index + 1);
		else if (key == Key::Left)
			index = max(1, index - 1);
		else
		{
			DateTime newtime = time;
			int8_t delta = key == Key::Up ? 1 : -1;
			if (index == 1)
				newtime.hour += delta * 10;
			else if (index == 2)
				newtime.hour += delta;
			else if (index == 3)
				newtime.minute += delta * 10;
			else if (index == 4)
				newtime.minute += delta;
			else if (index == 5)
				newtime.second += delta * 10;
			else if (index == 6)
				newtime.second += delta;
			else if (index == 7)
				newtime.year += delta * 10;
			else if (index == 8)
				newtime.year += delta;
			else if (index == 9)
				newtime.month += delta * 10;
			else if (index == 10)
				newtime.month += delta;
			else if (index == 11)
				newtime.day += delta * 10;
			else if (index == 12)
				newtime.day += delta;
			newtime.normalize();
			rtc.SetTime(newtime);
		}
	}
	if (index == 1 || index == 2)
		text.GoToXY(6 + index - 1, 4);
	else if (index == 3 || index == 4)
		text.GoToXY(6 + 1 + index - 1, 4);
	else if (index == 5 || index == 6)
		text.GoToXY(6 + 2 + index - 1, 4);
	else if (index == 7 || index == 8)
		text.GoToXY(7 + index - 7, 6);
	else if (index == 9 || index == 10)
		text.GoToXY(7 + 1 + index - 7, 6);
	else if (index == 11 || index == 12)
		text.GoToXY(7 + 2 + index - 7, 6);
	text.Print('^');
}

void App::PageAlarm(Event event, Key key, const DateTime& time)
{
	
}
