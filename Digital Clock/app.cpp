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
		if (key == Key::Down)
			index = min(1, index + 1);
		else if (key == Key::Up)
			index = max(0, index - 1);
	}
	text.GoToXY(0, index);
	text.Print('>');
}

void App::PageSetTime(Event event, Key key, const DateTime& time)
{
	if (event == Event::KeyPress)
	{
		current_page = Page::MainMenu;
	}
}

void App::PageAlarm(Event event, Key key, const DateTime& time)
{
	
}
