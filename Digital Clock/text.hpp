#pragma once

#include "display.hpp"

class Text
{
public:
	void Print(char c)
	{
		switch (c)
		{
			case '\n':
				GoToXY(0, Y() + 1);
				break;
			case '\r':
				GoToXY(0, Y());
				break;
			case '\b':
				GoToXY(X() - 1, Y());
				break;
			case '\t':
			{
				uint8_t x = X();
				for (uint8_t i = 0; i < 4 - x % 4; i++)
					Print(' ');
				break;
			}
			default:
				display.DrawProgMem((uint8_t*)&font[c - font_begin], 1, char_width);
		}
	}
	
	void Print(const char* str)
	{
		for (; *str != '\0'; str++)
			Print(*str);
	}
	
	void PrintProgMem(const char* str)
	{
		for (; ; str++)
		{
			char c = pgm_read_byte(str);
			if (c == '\0')
				break;
			Print(c);
		}
	}
	
	void GoToXY(uint8_t x, uint8_t y)
	{
		display.GoToXY(x * char_width, y);
	}
	
	uint8_t X() { return display.cursor_x / char_width; }
	uint8_t Y() { return display.cursor_y; }
	
private:
	static const uint8_t char_width = 6;
	static const char font_begin = ' ';
	static const char font[][6];
};

extern Text text;
