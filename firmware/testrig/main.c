#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "AS5043.h"
#include "uart.h"

const uint8_t degree_mode = 0;

int main(void) {

	uint16_t data;
	uint16_t rot;
	//uint8_t chr;
	char str[5];
	uart_init();

	AS5043_setup();

	DDRC = 0xFF;
	DDRD |= (1 << PD4);

	while(1) {

		data = AS5043_readout();

		if(AS5043_checkParity(data) && (uart_getc() != -1)) {

			rot = AS5043_getPos(data);

			if(degree_mode) {
				rot = (rot * 45) / 128;
			}

			itoa(rot, str, 10);
			uart_puts(str);
			uart_puts("\r\n");

			PORTD |= (1 << PD4);
			_delay_us(1);
			PORTD &= ~(1 << PD4);
		}
		_delay_ms(10);
	}


}
