#include "PNG.h"

// printf
int __io_putchar(int ch) {
	HAL_UART_Transmit(USER_UART, (uint8_t*) &ch, 1, 0xFFFF);
	return ch;
}

int _write(int file, char *ptr, int len) {
	HAL_UART_Transmit(USER_UART, (uint8_t*) ptr, len, 0xFFFF);
	return len;
}

// scanf
int __io_getchar(void) {
	uint8_t ch = 0;
	HAL_UART_Receive(USER_UART, &ch, 1, HAL_MAX_DELAY);
	HAL_UART_Transmit(USER_UART, &ch, 1, HAL_MAX_DELAY);
	return ch;
}

int _read(int file, char *ptr, int len) {
	int DataIdx = 0;
	char ch;

	for (; DataIdx < len; DataIdx++) {
		ch = __io_getchar();
		*ptr = ch;
		ptr++;
		if (ch == 13 || ch == 10) {
			*(ptr) = 0;
			break;
		}
	}
	return DataIdx + 1;
}
