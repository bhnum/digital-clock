#pragma once

#include <avr/pgmspace.h>
#include <string.h>
#include "i2c.hpp"

// OLED 128x64 display driver for SSD1306
class Display
{
public:
	static const uint8_t i2c_address = 0x3c;
	static const uint8_t display_width = 128;
	static const uint8_t display_height = 64;

	void Initialize(bool display_on = false);
	
	void TurnOn()
	{
		
	}
	
	void TurnOff()
	{
		
		
	}
	
	void SetContrst(uint8_t contrast)
	{
		
		
	}
	
	void Draw(uint8_t* bitmap, size_t height, size_t width)
	{
		
		
	}
	
	void DrawProgMem(uint8_t* bitmap, size_t height, size_t width)
	{
		
		
	}
	
	void SetCursor(uint8_t x, uint8_t y)
	{
		// TODO: talk to the chip
		cursor_x = x;
		cursor_y = y;
	}
	
	void GetCursor(uint8_t& x, uint8_t& y)
	{
		x = cursor_x;
		y = cursor_y;
	}
	
	void ClearScreen()
	{
		
		
	}

	void StartCommand()
	{
		i2c.Start();
		i2c.WriteAddress(i2c_address, true);
		i2c.Write(0x00);
	}
	
	void StartData()
	{
		i2c.Start();
		i2c.WriteAddress(i2c_address, true);
		i2c.Write(0x40);
	}
	
	void Send(uint8_t data)
	{
		i2c.Write(data);
	}
	
	void Stop()
	{
		i2c.Stop();
	}

private:
	static const uint8_t init_sequence[];
	uint8_t cursor_x = 0;
	uint8_t cursor_y = 0;
	
	enum Command : uint8_t
	{
		// ---- Fundamental Commands
		SetContrastControl = 0x81, // , contrast value
			
		// EntireDisplayOn | 0 : Output follows RAM content
		// EntireDisplayOn | 1 : Output ignores RAM content
		EntireDisplayOn = 0xa4,
			
		NormalDisplay = 0xa6,
		InverseDisplay = 0xa7,
			
		DisplayOff = 0xae, // Sleep mode
		DisplayOn = 0xaf,
			
			
		// ---- Scrolling Commands
		ContinuousHorizontalScrollSetup = 0x26, // refer to datasheet
		ContinuousVerticalAndHorizontalScrollSetup = 0x29, // refer to datasheet
		DeactivateScroll = 0x2e,
		ActivateScroll = 0x2f,
		SetVerticalScrollArea = 0xa3, // refer to datasheet
			
			
		// ---- Addressing Setting Command Table
			
		SetAddressingMode = 0x20, // , mode from the followings:
		HorizontalAddressingMode = 0x00,
		VerticalAddressingMode = 0x01,
		PageAddressingMode = 0x02,
			
		// These commands are only for page addressing mode
		SetLowerColumnStartAddress = 0x00, // | 4-bit address
		SetHigherColumnStartAddress = 0x10, // | 4-bit address
		SetPageStartAddress = 0xb0, // | 3-bit page start address
			
		// These commands are only for horizontal or vertical addressing mode
		SetColumnAddress = 0x21, // , 7-bit column start address, 6-bit column end address
		SetPageAddress = 0x22, // , 3-bit page start address, 3-bit page end address
			
			
		// ---- Hardware Configuration Commands
			
		SetDisplayStartLine = 0x40, // | 6-bit start line register
			
		// SetSegmentRemap | 0 : column address 0 is mapped to SEG0
		// SetSegmentRemap | 1 : column address 127 is mapped to SEG0
		SetSegmentRemap = 0xa0,
			
		// Set MUX ratio to N+1 MUX
		// N from 16MUX to 64MUX, RESET=111111b (i.e. 63d, 64MUX)
		// N from 0 to 14 is invalid.
		SetMultiplexRatio = 0xa8, // , 6-bit N
			
		// SetComOutputScanDirection | 0 : normal mode (RESET) Scan from COM0 to COM[N –1]
		// SetComOutputScanDirection | 8 : remapped mode. Scan from COM[N-1] to COM0
		SetComOutputScanDirection = 0xc0,
			
		// Set vertical shift by COM from 0 to 63
		SetDisplayOffset = 0xd3, // , 6-bit offset
			
		SetComPinsHardwareConfiguration = 0xda, // , config from the following:
		SequentialPinDisableLeftRightComRemap = 0x02,
		SequentialPinEnableLeftRightComRemap = 0x22,
		AlternativePinDisableLeftRightComRemap = 0x12, // RESET
		AlternativePinEnableLeftRightComRemap = 0x32,
			
			
		// ---- Timing & Driving Scheme Setting Commands
		SetDisplayClockDivideRatioAndOscillatorFrequency = 0xd5, // refer to datasheet
		SetPrechargePeriod = 0xd9, // refer to datasheet
		SetVComHDeselectLevel = 0xdb, // refer to datasheet
		Nop = 0xe3,
		
		// Charge Pump Setting Command
		ChargePumpSetting = 0x8d, // , setting one of the following:
		DisableChargePump = 0x10,
		EnableChargePump = 0x14,
	};
};

extern Display display;
