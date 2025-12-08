#include <avr/io.h>
#include "LED.h"
#include <util/delay.h>
#include <stdint.h>

void uart_init_slave(void) {
    uint16_t ubrr = 8;    // 115200 @ 16MHz
    UBRR0H = 0;
    UBRR0L = ubrr;

    UCSR0A = 0;
    UCSR0B = (1<<RXEN0); 
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);             // 8N1
}

uint8_t uart_poll_byte(uint8_t *out) {
    if (UCSR0A & (1<<RXC0)) {
        *out = UDR0;
        return 1;
    }
    return 0;
}

// main
void main (void) {
    uart_init_slave();
    init_strips();
    _delay_ms(10);

    uint8_t received;

    while (1) {
        if (uart_poll_byte(&received)) {
            uint8_t block = received & 0x0F;
            uint8_t solid = (received & 0x80) != 0;
            if (block < 9 && block >= 0) {
                  if (solid) {
                    led_show_block(block);
                } else {
                    led_show_block_hint(block);
                }
            }
        }
    }
}