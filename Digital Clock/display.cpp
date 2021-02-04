#include "display.hpp"
#include <util/delay.h>

const uint8_t Display::init_sequence[] PROGMEM = {
	DisplayOff,
	SetAddressingMode, HorizontalAddressingMode,
	SetPageStartAddress | 0,
	SetComOutputScanDirection | 8,
	SetLowerColumnStartAddress | 0,
	SetHigherColumnStartAddress | 0,
	SetDisplayStartLine | 0,
	SetContrastControl, 0x3f,
	SetSegmentRemap | 1,
	NormalDisplay,
	SetMultiplexRatio, 63, // display_height - 1
	EntireDisplayOn | 0,
	SetDisplayOffset, 0,
	SetDisplayClockDivideRatioAndOscillatorFrequency, 0xf0,
	SetPrechargePeriod, 0x22,
	SetComPinsHardwareConfiguration, AlternativePinDisableLeftRightComRemap,
	SetVComHDeselectLevel, 0x20, // 0.77 x Vcc
	ChargePumpSetting, EnableChargePump,
};

void Display::Initialize(bool display_on)
{
	_delay_ms(200);
	BeginCommand();
	for (size_t i = 0; i < sizeof(init_sequence); i++)
		Send(pgm_read_byte(&init_sequence[i]));
		
	if (display_on)
		Send(DisplayOn);
	else
		Send(DisplayOff);
	End();
}

Display display = Display();
