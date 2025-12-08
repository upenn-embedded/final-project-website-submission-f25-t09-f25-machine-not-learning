
#include <avr/io.h>

void uart_init_master(void) {
    // 115200 baud at 16 MHz
    uint16_t ubrr = 8;
    UBRR0H = 0;
    UBRR0L = ubrr;

    UCSR0A = 0;
    UCSR0B = (1<<TXEN0);
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);  // 8N1
}

void uart_send_byte(uint8_t b) {
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = b;
}

void uart_send_block_hint(uint8_t block) {
    uint8_t cmd = block & 0x0F;
//    for (uint8_t i=0; i<10; i++)
        uart_send_byte(cmd);
}

void uart_send_block_solid(uint8_t block) {
    uint8_t cmd = 0x80 | (block & 0x0F);
//    for (uint8_t i=0; i<10; i++)
        uart_send_byte(cmd);
}