/*
 * AS5043.c
 *
 *  Created on: Feb 28, 2012
 *      Author: sebastian
 */


#include "AS5043.h"


void AS5043_setup(void) {
	// Set MOSI and SCK, SS/CS output, all others input
	DDRB = (1<<PB3)|(1<<PB5) | (1<<PB2);
	// Enable SPI, Master, set clock rate fck/32
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0) | (1 << SPR1) | (1<<CPOL); // | (1 << DORD);
	// double spi speed for fck/8
	SPSR |= (1 << SPI2X);

	// Set SS/CS
	PORTB |= (1 << PB2);

	DDRC = 0xFF;
}

uint16_t AS5043_readout(void) {
	uint16_t data;

	//CS low
	PORTB &= ~(1 << PB2);

	// Write a dummy byte
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));
	// Read highbyte, shift left 9 because MSB bit is always 1
	data = SPDR << 9;

	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));
	// Read lowbyte, shift left 1 because LSB will be used to store parity
	data |= SPDR << 1;

	// 15 bit read so far

	// Get parity info and check it
	// Also we use the LSB in data to store the checks result,
	// so we can retrieve the later using AS5043_checkParity(uint16_t data)
	if(((PINB & (1 << PB4)) >> PB4) == AS5043_calcParity(data)) {
		data |= 1;
	}
	else {
		data &= ~1;
	}


	// CS high
	PORTB |= (1 << PB2);

	return data;

}


uint8_t AS5043_calcParity(uint16_t data) {
	uint8_t i,bit;
	bit = 0;
	for(i = 0; i < 15; i++) {
		data = data >> 1;		// We shift first since the LSB is the paritybit
		bit = bit ^ (data & 1);
	}

	return bit;
}
