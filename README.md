AS5043-Encoder
==============

This repos contains all the plans and software necessary to build a magnetic rotary encoder using the AS4053.

The setup for this chip is rather simple,
you need a few external components and a neodymium magnet that is attached to your rotating axis.
Then the sensor is placed below the magnet.
Since it uses the magnetic field neither dirt nor light will be a problem.
The sensor will output the angle of the magnet relative to a programmable starting position via analogue output
or via a [SSI](https://en.wikipedia.org/wiki/Synchronous_Serial_Interface)-Style interface.
(See the datasheet for details.)
With 10bit resolution it should be theoretically possible to measure the current angle in 0.35° steps.

The guys at austria microsystems were kind enough to send me 3 AS5043 and 3 AS5030 (similar part 8bit resolution).
*Big thanks to them.*


Mechanical Parts
----------------
There are 2 plans available :

* A version that can be build using a lathe
* A 3D printable version created with open scad
* A 3D printable version created with freecad

The plans for the first version can be found in the repo directory [chasis/lathe](chasis/lathe).
You can simply print out the pdfs and hand them over to someone who has access to a lathe.
He or she should know what to do with them.

For the 3D printable versions simply run the stl files through your 3D printing toolchain.
The 3D printable versions can be found in [chasis/3d_printable_scad](chasis/3d_printable_scad)
and [chasis/3d_printable_freecad](chasis/3d_printable_freecad).

You will also need some M3 screws and nuts which you should find in your favorite hardware store.
For the magnets I'd recommend browsing ebay for 5mmx5mmx5mm neodym magnet cubes.
The ball bearing you'll need is a common 6200Z bearing.
Again try your hardware store or ebay.
There you'll possible also find some M3 5mm long distance bolts, that are used in the lathe version of the chassis.


Software
--------
The software part required some tricks to get it working.
Comparing the signal diagrams for SPI in Atmels datasheet and for *SSI* in the AS5043 a small problem becomes obvious.
The AS5043 needs a clock signal that is high in idle stage and the bits will be shifted out on the positive clock edge.
That sounds very much like the SPI mode 3 in Atmel-speech: Clock high on idle and data will be read on every rising edge.
Leaving us with one problem: The Atmel hardware SPI can read the miso pin only at a clock edge.
As mentioned before data is sampled at the rising edge for mode 3.
So shifting out and reading would be done almost synchronous and due to signal rise and fall times this will probably fail 90% of the time.

The solution is to use mode 2: Clock high on idle and bits are read at the falling edge.
Now we have a different problem.
The first bit will be read before it was shifted out by the AS5043.
Using AVR SPI it is possible to read one byte at time, so two read cycles are required to read the full 16bit encoder output.

The first byte read in mode 2 will look like this:

| Bit 7       | Bit 6 | Bit 5 | Bit 4  | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
|-------------|-------|-------|--------|-------|-------|-------|-------|
| **garbage** | D9    | D8    | D7     | D6    | D5    | D4    | D3    |

Reading the next 8bit will work fine since the 8th bit was shifted out on the last rising edge after reading the first 7 bit.
It will be read as the bit in on the falling edge for the next byte,
which looks like this:

| Bit 7 | Bit 6 | Bit 5 | Bit 4  | Bit 3 | Bit 2 | Bit 1   | Bit 0   |
|-------|-------|-------|--------|-------|-------|---------|---------|
| D2    | D1    | D0    | OCF    | COF   | LIN   | MAG INC | MAG DEC |

Everything after the *D0*-bit is status information needed in order to decide whether the measurement was valid.
Unfortunately important status bit is still missing.
The 16th bit contains the parity information required to check against transmission errors.
It is shifted out on the last rising edge when reading the second byte. Since the clock line is high afterwards,
the data line will stay on the 16th bits level until the next read is performed.
It can read by manually checking the *MISO* pins state.
The rest is just nifty bit shifting.

It is also important not to use the SS of the AVR mcu for chip select,
since this pin will change to high after reading the first byte, which causes the AS5043 to reset its *SSI* interface.

The code for this can be found in [AS5043.c](firmware/lib/AS5043.c) and in [AS5043.h](firmware/lib/include/AS5043.h).

A simple code for testing can be found in the directory [firmware/demo](firmware/demo).
main.c contains code for an Atmega8 or Atmega328 that reads the encoder, calculates the rotation in degrees and sends out using the uart.
cube.py is a simple receiving is the receiving part running on your computer.
It uses PyOpenGL to display a wireframe cube and rotate it according to the encoders rotation value.
