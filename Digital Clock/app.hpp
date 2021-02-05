#pragma once

#include "keypad.hpp"
#include "rtc.hpp"
#include "text.hpp"
#include "beeper.hpp"

enum class Event : uint8_t
{
	Start = 0, Refresh, DrawOver, KeyPress, TimeChange
};

class App
{
public:
	void Run(Event event, Key key, const DateTime& time);
	
private:
	enum class Page : uint8_t
	{
		Void = 0, MainMenu, SetTime, Alarm, ManageAlarms, EditAlarm
	};
	
	Page current_page = Page::MainMenu;
	Page last_page = Page::MainMenu;
	
	void PageVoid(Event event, Key key, const DateTime& time);
	void PageMainMenu(Event event, Key key, const DateTime& time);
	void PageSetTime(Event event, Key key, const DateTime& time);
	void PageAlarm(Event event, Key key, const DateTime& time);
	void PageManangeAlarms(Event event, Key key, const DateTime& time);
	void PageEditAlarm(Event event, Key key, const DateTime& time);
};