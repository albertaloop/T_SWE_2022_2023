#ifndef __PERIPH_IF__
#define __PERIPH_IF__

#include <stdint.h>

void init_uart(void * uh);

int read_uart(uint8_t *buf, uint16_t size, uint32_t timeout);

int write_uart(uint8_t *buf, uint16_t size, uint32_t timeout);

#endif
