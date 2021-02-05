#include "app.hpp"
#include <stdlib.h>
#include <util/delay.h>
#include "alarm.hpp"

static Alarm current_alarm;

// helper functions
static void print_time(const DateTime& time, bool seconds = true)
{
	text.Print('0' + time.hour / 10);
	text.Print('0' + time.hour % 10);
	text.Print(':');
	text.Print('0' + time.minute / 10);
	text.Print('0' + time.minute % 10);
	if (seconds)
	{
		text.Print(':');
		text.Print('0' + time.second / 10);
		text.Print('0' + time.second % 10);
	}
}

static void print_date(const DateTime& date)
{
	text.PrintProgMem(PSTR("20"));
	text.Print('0' + date.year / 10);
	text.Print('0' + date.year % 10);
	text.Print('-');
	text.Print('0' + date.month / 10);
	text.Print('0' + date.month % 10);
	text.Print('-');
	text.Print('0' + date.day / 10);
	text.Print('0' + date.day % 10);
}

static inline uint8_t time_string_xpos_index(uint8_t index)
{
	return index / 2 + index;
}

static inline uint8_t date_string_xpos_index(uint8_t index)
{
	return 2 + index / 2 + index;
}

void App::Run(Event event, Key key, const DateTime& time)
{
	if (time.second == 0)
	{
		size_t count = alarm_manager.GetCount();
		for (size_t i = 0; i < count; i++)
		{
			Alarm alarm;
			alarm_manager.GetAlarm(i, alarm);
			bool fire_alarm = alarm.time == time;
			if (alarm.option == AlarmOption::EveryDay)
				fire_alarm = alarm.time.hour == time.hour && alarm.time.minute == time.minute;
			if (fire_alarm)
			{
				current_alarm = alarm;
				if (current_page != Page::Alarm)
				{
					last_page = current_page;
					current_page = Page::Alarm;
				}
				if (last_page == Page::Void)
					display.TurnOn();
				PageAlarm(Event::Start, Key::None, time);
				break;
			}
		}
	}
	
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
	case Page::Alarm:
		PageAlarm(event, key, time);
		break;
	case Page::ManageAlarms:
		PageManangeAlarms(event, key, time);
		break;
	case Page::EditAlarm:
		PageEditAlarm(event, key, time);
		break;
	}
	
	if (last_page != current_page)
	{
		if (current_page != Page::Alarm)
		{
			if (last_page != Page::Alarm)
			{
				last_page = current_page;
				Run(Event::Start, Key::None, time);
			}
			else
			{
				last_page = current_page;
				Run(Event::Refresh, Key::None, time);
			}
		}
	}
}

void App::PageVoid(Event event, Key key, const DateTime& time)
{
	if (event == Event::Start || event == Event::Refresh)
	{
		display.Clear();
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
	
	if (event == Event::Start || event == Event::Refresh)
	{
		display.Clear();
		text.PrintProgMem(PSTR(" Set Time\n"));
		text.PrintProgMem(PSTR(" Alarms\n"));
		text.PrintProgMem(PSTR(" Display Off\n"));
	}
	
	if (event == Event::Start || event == Event::Refresh || event == Event::TimeChange)
	{
		text.GoToXY(6, 4);
		print_time(time);
		
		text.GoToXY(6, 6);
		text.PrintProgMem(DateTime::DayName[time.day_of_week - 1]);
		text.PrintProgMem(PSTR("   "));
		
		text.GoToXY(5, 7);
		print_date(time);
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
				current_page = Page::ManageAlarms;
			else if (index == 2)
				current_page = Page::Void;
		}
		else if (key == Key::Down)
			index = min(2, index + 1);
		else if (key == Key::Up)
			index = max(0, index - 1);
	}
	text.GoToXY(0, index);
	text.Print('>');
}

void App::PageSetTime(Event event, Key key, const DateTime& time)
{
	static int8_t index = 0;
	
	if (event == Event::Start)
	{
		index = 0;
	}
	
	if (event == Event::Start || event == Event::Refresh)
	{
		display.Clear();
		text.PrintProgMem(PSTR("Press Ok to return"));
	}
	
	if (event == Event::Start || event == Event::Refresh || event == Event::TimeChange)
	{
		text.GoToXY(6, 2);
		print_time(time);
		
		text.GoToXY(6, 4);
		text.PrintProgMem(DateTime::DayName[time.day_of_week - 1]);
		text.PrintProgMem(PSTR("   "));
		
		text.GoToXY(5, 6);
		print_date(time);
	}
	
	if (event == Event::KeyPress)
	{
		if (index < 6)
			text.GoToXY(6 + time_string_xpos_index(index), 3);
		else if (index == 6)
			text.GoToXY(6, 5);
		else
			text.GoToXY(5 + date_string_xpos_index(index - 7), 7);
		text.Print(' ');
		
		if (key == Key::Ok)
			current_page = Page::MainMenu;
		else if (key == Key::Right)
			index = min(12, index + 1);
		else if (key == Key::Left)
			index = max(0, index - 1);
		else
		{
			DateTime newtime = time;
			int8_t delta = key == Key::Up ? 1 : -1;
			if (index == 0)
				newtime.hour += delta * 10;
			else if (index == 1)
				newtime.hour += delta;
			else if (index == 2)
				newtime.minute += delta * 10;
			else if (index == 3)
				newtime.minute += delta;
			else if (index == 4)
				newtime.second += delta * 10;
			else if (index == 5)
				newtime.second += delta;
			else if (index == 6)
				newtime.day_of_week += delta;
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
	if (index < 6)
		text.GoToXY(6 + time_string_xpos_index(index), 3);
	else if (index == 6)
		text.GoToXY(6, 5);
	else
		text.GoToXY(5 + date_string_xpos_index(index - 7), 7);
	text.Print('^');
}

void App::PageAlarm(Event event, Key key, const DateTime& time)
{
	if (event == Event::Start)
	{
		beeper.TurnOn();
		display.Clear();
		text.GoToXY(4, 1);
		text.PrintProgMem(PSTR("-- Alarm --"));
		
		text.GoToXY(7, 3);
		print_time(current_alarm.time, false);
		
		if (current_alarm.option == AlarmOption::Once)
		{
			text.GoToXY(5, 5);
			print_date(current_alarm.time);
		}
		
		text.GoToXY(6, 7);
		text.PrintProgMem(PSTR("Turn off"), true);
	}
	
	if (event == Event::KeyPress)
	{
		beeper.TurnOff();
		current_page = last_page;
		last_page = Page::Alarm;
	}
}

// -1 means new alarm
static int8_t edit_alarm_index;

void App::PageManangeAlarms(Event event, Key key, const DateTime& time)
{
	static uint8_t index = 0;
	
	if (event == Event::Start)
	{
		edit_alarm_index = 0;
		index = 0;
	}
	
	uint8_t count = alarm_manager.GetCount();
	if (event == Event::Start || event == Event::Refresh)
	{
		display.Clear();
		text.PrintProgMem(PSTR("Alarms\n"));
		text.PrintProgMem(PSTR(" Back"));
		if (count < 6)
		{
			text.GoToXY(10, 1);
			text.PrintProgMem(PSTR(" New"));
		}
		for (uint8_t i = 0; i < count && i < 6; i++)
		{
			Alarm alarm;
			alarm_manager.GetAlarm(i, alarm);
			if (alarm.option == AlarmOption::Once)
			{
				text.Print("\n ");
				print_date(alarm.time);
			}
			else if (alarm.option == AlarmOption::EveryDay)
			{
				text.PrintProgMem(PSTR("\n Every day "));
			}
			text.Print(' ');
			print_time(alarm.time, false);
		}
	}
	
	if (event == Event::KeyPress)
	{
		if (key == Key::Ok)
		{
			if (index == 0)
				current_page = Page::MainMenu;
			else if (index == 1)
			{
				edit_alarm_index = -1;
				current_page = Page::EditAlarm;
			}
			else
			{
				edit_alarm_index = index - 2;
				current_page = Page::EditAlarm;
			}
		}
		else
		{
			if (index == 0)
				text.GoToXY(0, 1);
			else if (index == 1)
				text.GoToXY(10, 1);
			else
				text.GoToXY(0, index);
			text.Print(' ');
			if (key == Key::Down)
			{
				index = min(index + 1, count + 1);
				if (index == 1 && count >= 6)
					index = min(index + 1, count + 1);
			}
			else if (key == Key::Up)
			{
				index = max(0, index - 1);
				if (index == 1 && count >= 6)
					index = max(0, index - 1);
			}
		}
	}
	if (index == 0)
		text.GoToXY(0, 1);
	else if (index == 1)
		text.GoToXY(10, 1);
	else
		text.GoToXY(0, index);
	text.Print('>');
}

void App::PageEditAlarm(Event event, Key key, const DateTime& time)
{
	static Alarm alarm;
	static uint8_t index = 0;
	
	if (event == Event::Start)
	{
		if (edit_alarm_index == -1)
		{
			alarm.time = time;
			alarm.time.second = 0;
			alarm.option = AlarmOption::Once;
		}
		else
		{
			alarm_manager.GetAlarm(edit_alarm_index, alarm);
		}
		index = 0;
	}
	
	if (event == Event::Start || event == Event::Refresh)
	{
		display.Clear();
		if (edit_alarm_index == -1)
			text.PrintProgMem(PSTR("New Alarm"));
		else
			text.PrintProgMem(PSTR("Edit Alarm"));
		text.GoToXY(14, 0);
		text.PrintProgMem(PSTR(" Back\n"));
		
		if (edit_alarm_index != -1)
		{
			text.PrintProgMem(PSTR(" Change\t\t Delete"));
		}
		else
		{
			text.PrintProgMem(PSTR(" Create"));
		}
		
		text.GoToXY(1, 2);
		text.PrintProgMem(PSTR("Once"), alarm.option == AlarmOption::Once);
		text.GoToXY(12, 2);
		text.PrintProgMem(PSTR("Every day"), alarm.option == AlarmOption::EveryDay);
		
		text.GoToXY(7, 4);
		print_time(alarm.time, false);
		
		if (alarm.option == AlarmOption::Once)
		{
			text.GoToXY(5, 6);
			print_date(alarm.time);
		}
	}
	
	if (event == Event::KeyPress)
	{
		if (key == Key::Ok)
		{
			if (index == 0) // Back
				current_page = Page::ManageAlarms;
			else if (index == 1 || index > 4) // Create/Change
			{
				if (edit_alarm_index == -1)
					alarm_manager.AddAlarm(alarm);
				else
					alarm_manager.UpdateAlarm(edit_alarm_index, alarm);
				current_page = Page::ManageAlarms;
			}
			else if (index == 2) // Delete
			{
				alarm_manager.RemoveAlarm(edit_alarm_index);
				current_page = Page::ManageAlarms;
			}
			else if (index == 3) // Once
			{
				alarm.option = AlarmOption::Once;
			}
			else if (index == 4) // Every day
			{
				alarm.option = AlarmOption::EveryDay;
			}
		}
		else if (key == Key::Right || key == Key::Left ||
			(index < 5 && (key == Key::Up || key == Key::Down)))
		{
			uint8_t max_index = alarm.option == AlarmOption::Once ? 14 : 8;
			if (key == Key::Right || key == Key::Down)
			{
				index = min(max_index, uint8_t(index + 1));
				if (index == 2 && edit_alarm_index == -1)
					index++;
			}
			else if (key == Key::Left || key == Key::Up)
			{
				index = max(0, index - 1);
				if (index == 2 && edit_alarm_index == -1)
					index--;
			}
		}
		else
		{			
			int8_t delta = key == Key::Up ? 1 : -1;
			if (index == 5)
				alarm.time.hour += delta * 10;
			else if (index == 6)
				alarm.time.hour += delta;
			else if (index == 7)
				alarm.time.minute += delta * 10;
			else if (index == 8)
				alarm.time.minute += delta;
			else if (index == 9)
				alarm.time.year += delta * 10;
			else if (index == 10)
				alarm.time.year += delta;
			else if (index == 11)
				alarm.time.month += delta * 10;
			else if (index == 12)
				alarm.time.month += delta;
			else if (index == 13)
				alarm.time.day += delta * 10;
			else if (index == 14)
				alarm.time.day += delta;
			alarm.time.normalize();
		}
		PageEditAlarm(Event::Refresh, Key::None, time);
	}
	
	if (index == 0)
		text.GoToXY(14, 0);
	else if (index == 1)
		text.GoToXY(0, 1);
	else if (index == 2)
		text.GoToXY(12, 1);
	else if (index == 3)
		text.GoToXY(0, 2);
	else if (index == 4)
		text.GoToXY(11, 2);
	else if (index <= 8)
		text.GoToXY(7 + time_string_xpos_index(index - 5), 5);
	else
		text.GoToXY(5 + date_string_xpos_index(index - 9), 7);
	text.Print(index < 5 ? '>' : '^');
}
