#ifndef UART_SYNC_MASTER_H
#define UART_SYNC_MASTER_H

#include <stdint.h>

void uart_init_master(void);
void uart_send_byte(uint8_t b);
void uart_send_block_hint(uint8_t block);
void uart_send_block_solid(uint8_t block);

#endif // UART_SYNC_MASTER_H