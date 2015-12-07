/*
 * AS5043.h
 *
 *  Created on: Feb 28, 2012
 *      Author: sebastian
 */

#ifndef AS5043_H_
#define AS5043_H_ AS5043_H_

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"



void AS5043_setup(void);
uint16_t AS5043_readout(void);

uint8_t AS5043_calcParity(uint16_t data);


/*
 * Helper functions to retrieve single bits from AS5043_readout()
 * Format used in the unit16_t returned by AS5043_readout:
 * D9 D8 D7 D6 D5 D4 D3 D2 D1 D0 OCF COF LIN INC DEC ParityCheckresult 
 */

static inline uint8_t AS5043_checkParity(uint16_t data) {
		return data & 1;
}

static inline uint8_t AS5043_getDEC(uint16_t data) {
		return data & (1 << 2);
}

static inline uint8_t AS5043_getINC(uint16_t data) {
		return data & (1 << 3);
}

static inline uint8_t AS5043_getLIN(uint16_t data) {
		return data & (1 << 4);
}

static inline uint8_t AS5043_getCOF(uint16_t data) {
		return data & (1 << 5);
}

static inline uint8_t AS5043_getOCF(uint16_t data) {
		return data & (1 << 6);
}

static inline uint16_t AS5043_getPos(uint16_t data) {
		return (data & 0xFFC0) >> 6;
}


#endif /* AS5043_H_ */
