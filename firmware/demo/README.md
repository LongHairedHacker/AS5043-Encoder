Running the Demo
================

Compiling
---------
To compile this example you will need make, avrgcc avr-binutils and the avrlibc.
If you are reading this I assume you already have an avr toolchain in place.
Also you'll need avrdude to flash your avr.

The makefile takes 3 optional parameters:
 * **F_CPU** Clock speed of your controller in Hertz (default: 16000000)
 * **AVRMCU** The name of your microcontroller, currently only atmega8 and atmega32p are supported (default: atemga8)
 * **ISPPORT** The serial interface for `make flash` (default: /dev/ttyACM0)

 To compile and flash the code run (assuming there is a serial bootloader like optiboot present on your controller):
 ```
 make
 make flash
 ```

 If you want to run the code on your Arduino try:
 ```
 make AVRMCU=atmega32p ISPPORT=/dev/ttyACM0
 make AVRMCU=atmega32p ISPPORT=/dev/ttyACM0 flash
 ```

Afterwards connect the encoder and start a terminal emulator of your choice on your serial interface.
Configure it for **38400** Baud, **8** Bit, **N**o Parity and **1** stop bit.
If you start sending newlines to the microcontroller, you should see it respond with measurements in plain ASCII.
If your are seeing binary garbage, you either have the wrong baudrate or degree_mode not set to 1.

You can also run cube_demo.py on this interface if you have PyOpengl and PySerial installed.
It might be necessary to change the SERIAL constant on line 16 to the correct interface.

Connecting the encoder
----------------------

Simply connect the encoder according to the following table:

| AS5043 | Breakout Pin | AVR        | Arduino        |
|--------|--------------|------------|----------------|
| PROG   | 1            | GND        | GND            |
| DO     | 3            | PB4 (MISO) | digital pin 12 |
| CLK    | 5            | PB5 (SCK)  | digital pin 13 |
| CS     | 7            | PB2 (SS)   | digital pin 10 |
| MODE   | 9            | GND        | GND            |
| VDD5V  | 2            | +5v        | +5v            |
| VSS    | 4            | GND        | GND            |
