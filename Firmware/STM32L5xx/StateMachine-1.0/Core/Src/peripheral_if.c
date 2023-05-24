#include "peripheral_if.h"
#include "stm32l5xx_hal.h"

void * uart_handle;

void init_uart(void * uh) {
	uart_handle = uh;
}

int read_uart(uint8_t *buf, uint16_t size, uint32_t timeout) {
	HAL_UART_Receive(uart_handle, buf, size, timeout);

}

int write_uart(uint8_t *buf, uint16_t size, uint32_t timeout) {
	  HAL_UART_Transmit(uart_handle, buf, size, timeout);

}



