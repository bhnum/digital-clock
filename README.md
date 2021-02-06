# Digital Clock

Digital clock project for microcontroller course written in Atmel Studio. You can simulate it using the Proteus project file, or you can program it on an ATmega on board.

#### Requirements

- ATmega88 or any other ATmega microcontroller with at least 8Kb program memory
- DS1307 Real Time Clock (with optional 3V lithium coin cell battery)
- 32.768KHz clock crystal
- SSD1306 0.96" 128x64 OLED graphic display
- Passive buzzer module
- 6x push buttons for navigation and reset
- 4x 2N2007 or N-channel MOSFET capable of at least 120mA of drain current
- 4-digit 7-segment
- Some resistors and capacitors
- BMP180 pressure and temperature sensor *optional (TODO)
- Photocell or photodiode for ambient light sensor *optional (TODO)
- Micro USB port *optional
- On-off switch *optional

Some codes are different for simulation and real hardware to increase Proteus simulation speed. For this the `SIMULATION` preprocessor flag is defined. Disable the `SIMULATION` flag before compiling for real hardware. Also the MCU clock needs to be 8MHz for the code to function properly.

Some pictures of working hardware:

![Side View](/Image/side-view.jpg)

![Top view](/Image/top-view.jpg)

## Simulation

This project can be simulated using the Proteus simulation file. An image of the simulation is shown below.

Note: This file is different from the schematic for real hardware; It just works in simulation.

![Simulation in Proteus](/Image/simulation.png)

## To-do List

- Change code to make it work for other frequencies than 8MHz
- Automatically change display and 7-segment brightness depending on ambient light level using a photodiode.
- Show pressure and temperature on the display
- Additional settings menu