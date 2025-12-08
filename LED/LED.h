
#ifndef LED_H
#define LED_H

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define LED_NUM     60 

#define CYCLES_T0H 5    // 375 ns
#define CYCLES_T1H 10   // 687 ns
#define CYCLES_T0L 14   // 812 ns
#define CYCLES_T1L 15    // 562 ns

#define STRIP0_PORT PORTC
#define STRIP0_DDR  DDRC
#define STRIP0_PIN  PC0

#define STRIP1_PORT PORTC
#define STRIP1_DDR  DDRC
#define STRIP1_PIN  PC1

#define STRIP2_PORT PORTC
#define STRIP2_DDR  DDRC
#define STRIP2_PIN  PC2

#define STRIP3_PORT PORTC
#define STRIP3_DDR  DDRC
#define STRIP3_PIN  PC3

#define STRIP_HIGH(PORT, PIN)   ((PORT) |=  (1 << (PIN)))
#define STRIP_LOW(PORT, PIN)    ((PORT) &= ~(1 << (PIN)))

#define SEND_BIT(PORT, PIN, bit) do {                     \
    STRIP_HIGH(PORT, PIN);                                \
    if (bit) { __builtin_avr_delay_cycles(CYCLES_T1H); }  \
    else      { __builtin_avr_delay_cycles(CYCLES_T0H); } \
    STRIP_LOW(PORT, PIN);                                 \
    if (bit) { __builtin_avr_delay_cycles(CYCLES_T1L); }  \
    else      { __builtin_avr_delay_cycles(CYCLES_T0L); } \
} while (0)

static inline void send_byte(volatile uint8_t *port, uint8_t pin, uint8_t byte) {
    for (int8_t i = 7; i >= 0; i--) {
        SEND_BIT(*port, pin, (byte >> i) & 1);
    }
}

static inline void send_byte0(uint8_t b) { send_byte(&STRIP0_PORT, STRIP0_PIN, b); }
static inline void send_byte1(uint8_t b) { send_byte(&STRIP1_PORT, STRIP1_PIN, b); }
static inline void send_byte2(uint8_t b) { send_byte(&STRIP2_PORT, STRIP2_PIN, b); }
static inline void send_byte3(uint8_t b) { send_byte(&STRIP3_PORT, STRIP3_PIN, b); }

 #define SEND_LED0(r,g,b,w) do{ send_byte0(g); send_byte0(r); send_byte0(b); send_byte0(w);}while(0)
 #define SEND_LED1(r,g,b,w) do{ send_byte1(g); send_byte1(r); send_byte1(b); send_byte1(w);}while(0)
 #define SEND_LED2(r,g,b,w) do{ send_byte2(g); send_byte2(r); send_byte2(b); send_byte2(w);}while(0)
 #define SEND_LED3(r,g,b,w) do{ send_byte3(g); send_byte3(r); send_byte3(b); send_byte3(w);}while(0)

#define RESET_STRIP0() do{ STRIP_LOW(STRIP0_PORT, STRIP0_PIN); _delay_us(320); }while(0)
#define RESET_STRIP1() do{ STRIP_LOW(STRIP1_PORT, STRIP1_PIN); _delay_us(320); }while(0)
#define RESET_STRIP2() do{ STRIP_LOW(STRIP2_PORT, STRIP2_PIN); _delay_us(320); }while(0)
#define RESET_STRIP3() do{ STRIP_LOW(STRIP3_PORT, STRIP3_PIN); _delay_us(320); }while(0)

void init_strips(void);
void clear_all_strips(void);
void led_set_strip_segment(uint8_t strip, uint8_t segment);
void led_set_strip_segment_hint(uint8_t strip, uint8_t segment);
void led_set_block(uint8_t block);
void led_set_block_hint(uint8_t block);
void led_show_block(uint8_t block);
void led_show_block_hint(uint8_t block);

#endif