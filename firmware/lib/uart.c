#include "uart.h"

uint8_t uart_timed_out = 0;

void uart_init(void) {
	*uart_ubrrh = UBRR_VAL >> 8;		//Setting baudrate
	*uart_ubrrl = UBRR_VAL & 0xFF;

	*uart_ucsrb |= uart_txen | uart_rxen;  // uart rx and tx enable
	*uart_ucsrc = uart_ucsz;  // Asynchronous 8N1

	// flush receive buffer
    do
    {
        *uart_udr;
    }
    while (*uart_ucsra & uart_rxc);

	//reset tx and rx completeflags and udr empty flag
	*uart_ucsra = uart_rxc | uart_txc | uart_udre;
}

uint8_t uart_getc_timeout(void) {
	uint8_t retries = UART_TIMEOUT;
	uint8_t delays = 0;

	while (!(*uart_ucsra & uart_rxc) && (retries > 0)) {
		if(delays == 0) {
			retries--;
		}
		delays = (delays + 1) % 250;
		_delay_us(4);
	}

	if(retries > 0) {
		uart_timed_out = 0;
		return *uart_udr;
	}

	uart_timed_out = 1;
	return 0;
}

uint8_t uart_get_line(char buffer[], uint8_t maxlen) {
	char t = 0;
	uint8_t pos = 0;
	buffer[0] = 0;

	//maxlen needs to be at least big enough for one character + null byte.
	if(maxlen < 2) {
		return 0;
	}

	uart_clear_time_out();

	while(pos < maxlen && t != '\n' && !uart_has_timed_out()) {
		t = uart_getc_timeout();
		buffer[pos] = t;
		pos++;
	}

	// We passed the loop at least once, so pos can not be 0
	if(buffer[pos-1] != '\n') {
		return 0;
	}
	buffer[pos-1] = 0;
	return 1;
}
