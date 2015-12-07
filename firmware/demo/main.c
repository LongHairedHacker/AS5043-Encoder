#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "AS5043.h"
#include "uart.h"

const uint8_t degree_mode = 0;

int main(void) {

	uint16_t data;
	uint16_t rot;
	char str[5];
	uart_init();

	AS5043_setup();

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
		}
		_delay_ms(10);
	}
}
