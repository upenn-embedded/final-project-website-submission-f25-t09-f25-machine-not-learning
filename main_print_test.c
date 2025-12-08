/*
* File:   main.c
* Author: Xiang Ding
*
* Created on November 19, 2025, 10:24 AM
*/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include <stdlib.h>
#include "uart.h"

#include "mario_melody.h"

#include "LCD_GFX.h"
#include "ST7735.h"

#define ADC_PIN PC0
#define MUX_S0_PIN PB0
#define MUX_S1_PIN PB1
#define MUX_S2_PIN PB2

//#define LED_PORT PORTB
//#define LED_DDR  DDRB
//#define LED_PIN  PB5

#define PWM_PIN  PD5
#define NUM_BLOCK 9

//volatile uint16_t score = 0;
//volatile uint8_t wins = 0;
//volatile uint8_t round = 0;

volatile uint16_t remaining_ms = 0;
volatile uint16_t adc_val = 0;

static float midi_to_freq(uint8_t midi);
static uint16_t freq_to_top(float freq);

void gpio_init(){
    
    DDRB |= (1 << MUX_S0_PIN)|(1 << MUX_S1_PIN)|(1 << MUX_S2_PIN); // output
    PORTB &= ~(1 << MUX_S0_PIN);
    PORTB &= ~(1 << MUX_S1_PIN);
    PORTB &= ~(1 << MUX_S2_PIN);

}

void adc_switch_channel(uint8_t adc_channel){
    switch(adc_channel){
//        case 0:
//        // ADC0
//        ADMUX &= ~(1 << MUX0); // select the ADC channel to read
//        ADMUX &= ~(1 << MUX1);
//        ADMUX &= ~(1 << MUX2);
//        ADMUX &= ~(1 << MUX3);
////        printf("WE ARE AT CHANNEL: %u\n", adc_channel);
////        _delay_us(50);
//        break;
//
//        case 1:
//        // ADC1
//        ADMUX |= (1 << MUX0); // select the ADC channel to read
//        ADMUX &= ~(1 << MUX1);
//        ADMUX &= ~(1 << MUX2);
//        ADMUX &= ~(1 << MUX3);
////        printf("WE ARE AT CHANNEL: %u\n", adc_channel);
////        _delay_us(50);
//        break;
//    
//        case 2:
//        // ADC2
//        ADMUX &= ~(1 << MUX0); // select the ADC channel to read
//        ADMUX |= (1 << MUX1);
//        ADMUX &= ~(1 << MUX2);
//        ADMUX &= ~(1 << MUX3);
////        printf("WE ARE AT CHANNEL: %u\n", adc_channel);
////        _delay_us(50);
//        break;
//    
//        case 3:
//        // ADC3
//        ADMUX |= (1 << MUX0); // select the ADC channel to read
//        ADMUX |= (1 << MUX1);
//        ADMUX &= ~(1 << MUX2);
//        ADMUX &= ~(1 << MUX3);
////        printf("WE ARE AT CHANNEL: %u\n", adc_channel);
////        _delay_us(50);
//        break;
//        
//        case 4:
//        // ADC4
//        ADMUX &= ~(1 << MUX0); // select the ADC channel to read
//        ADMUX &= ~(1 << MUX1);
//        ADMUX |= (1 << MUX2);
//        ADMUX &= ~(1 << MUX3);
////        printf("WE ARE AT CHANNEL: %u\n", adc_channel);
////        _delay_us(50);
//        break;
//        
//        case 5:
//        // ADC5
//        ADMUX |= (1 << MUX0); // select the ADC channel to read
//        ADMUX &= ~(1 << MUX1);
//        ADMUX |= (1 << MUX2);
//        ADMUX &= ~(1 << MUX3);
////        printf("WE ARE AT CHANNEL: %u\n", adc_channel);
////        _delay_us(50);
//        break;
//        
//        case 6:
//        // ADC6
//        ADMUX &= ~(1 << MUX0); // select the ADC channel to read
//        ADMUX |= (1 << MUX1);
//        ADMUX |= (1 << MUX2);
//        ADMUX &= ~(1 << MUX3);
////        printf("WE ARE AT CHANNEL: %u\n", adc_channel);
////        _delay_us(50);
//        break;
        
        case 7:
        // ADC7
        PORTB &= ~(1 << MUX_S0_PIN);
        PORTB &= ~(1 << MUX_S1_PIN);
        PORTB &= ~(1 << MUX_S2_PIN);
        ADMUX |= (1 << MUX0); // select the ADC channel to read
        ADMUX |= (1 << MUX1);
        ADMUX |= (1 << MUX2);
        ADMUX &= ~(1 << MUX3);
//        printf("WE ARE AT CHANNEL 7\n");
//        _delay_us(50);
        break;
        
        case 8:
        // ADC8
        PORTB |= (1 << MUX_S0_PIN);
        PORTB &= ~(1 << MUX_S1_PIN);
        PORTB &= ~(1 << MUX_S2_PIN);
        ADMUX |= (1 << MUX0); // select the ADC channel to read
        ADMUX |= (1 << MUX1);
        ADMUX |= (1 << MUX2);
        ADMUX &= ~(1 << MUX3);
//        printf("WE ARE AT CHANNEL 8\n");
//        _delay_us(50);
        break;
        
        default:
//        ADMUX &= ~(1 << MUX0); // select the ADC channel to read
//        ADMUX &= ~(1 << MUX1);
//        ADMUX &= ~(1 << MUX2);
//        ADMUX &= ~(1 << MUX3);
        PORTB &= ~(1 << MUX_S0_PIN);
        PORTB &= ~(1 << MUX_S1_PIN);
        PORTB &= ~(1 << MUX_S2_PIN);
        ADMUX |= (1 << MUX0); // select the ADC channel to read
        ADMUX |= (1 << MUX1);
        ADMUX |= (1 << MUX2);
        ADMUX &= ~(1 << MUX3);
//        printf("WE ARE AT CHANNEL DEFAULT");
//        _delay_us(50);
    }
}

void adc_init(){
    DDRC &= ~(1 << ADC_PIN); // set PC0 as an input

    PRR0 &= ~(1 << PRADC); // clear the ADC power reduction bit by writing a logic zero to it in the power reduction register, this turns the ADC on

    ADMUX |= (1 << REFS0); // select the reference voltage
    ADMUX &= ~(1 << REFS1);
    
    ADCSRA |= (1 << ADPS0);// set up the ADC clock (prescalers), must be within 50kHz to 200kHz
    ADCSRA |= (1 << ADPS1);
    ADCSRA |= (1 << ADPS2);
    
    adc_switch_channel(0); // ADC0

    ADCSRA |= (1 << ADATE);

    ADCSRB &= ~(1 << ADTS0); // ADTS[2:0} = 000, Free Running Mode, constantly sampling and updating
    ADCSRB &= ~(1 << ADTS1);
    ADCSRB &= ~(1 << ADTS2);

    DIDR0 |= (1 << ADC0D); // disable input buffer

    ADCSRA |= (1 << ADEN); // enable ADC

    ADCSRA |= (1 << ADSC); // start conversation
}

uint16_t adc_read() {
    uint8_t low  = ADCL;
    uint8_t high = ADCH;
    return (high << 8) | low;
}

// WS2812 timing constants (in ns)
#define T0H  350  // 0 bit high time
#define T1H  700  // 1 bit high time
#define T0L  800  // 0 bit low time
#define T1L  600  // 1 bit low time
#define RES  80 // reset time (>50us)

//void led_init() {
//    LED_DDR |= (1 << LED_PIN); // Set pin as output
//    LED_PORT &= ~(1 << LED_PIN); // Low initially
//}
//
//void send_bit(uint8_t bit_val) {
//    if(bit_val) {
//        // send '1'
//        LED_PORT |= (1 << LED_PIN);
//        _delay_us(0.7); // 700ns ? 0.7us
//        LED_PORT &= ~(1 << LED_PIN);
//        _delay_us(0.6); // 600ns ? 0.6us
//    } else {
//        // send '0'
//        LED_PORT |= (1 << LED_PIN);
//        _delay_us(0.35); // 350ns ? 0.35us
//        LED_PORT &= ~(1 << LED_PIN);
//        _delay_us(0.8);  // 800ns ? 0.8us
//    }
//}
//
//void send_byte(uint8_t byte) {
//    for(int8_t i = 7; i >= 0; i--) {
//        send_bit(byte & (1 << i));
//    }
//}
//
//// For RGBW strip: 4 bytes per LED
//void send_led(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
//    send_byte(g); // SK6812 order: G R B W
//    send_byte(r);
//    send_byte(b);
//    send_byte(w);
//}
//
//void led_reset() {
//    LED_PORT &= ~(1 << LED_PIN);
//    _delay_us(RES); // >50us reset
//}

//void update_LCD(){
//   
//    // Draw the scores
//    LCD_drawChar(40, 40, '0' + score, WHITE, BLACK);
//    
//    // Draw the number of rounds won
//    LCD_drawChar(40, 60, '0' + wins, WHITE, BLACK);
//    
//     // Draw the round number
//    LCD_drawChar(80, 20, '0' + round, WHITE, BLACK);
//}

void buzzer_init(void) {
    
    cli();
    
    DDRD |= (1 << PWM_PIN);
    PORTD &= ~(1 << PWM_PIN);
    
    TCCR0A |= (1 << WGM00);
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << WGM02);
    
    TCCR0A |= (1 << COM0B1);

    TCCR0B |= (1 << CS02) | (1 << CS00);
  
    // initialize top and duty
    OCR0A = 78; 
    OCR0B = 39; // 50% duty cycle
    
    OCR0B = 0;
    
    sei();
}

void timer1_init(void)
{
    TCCR1A = 0;
    TCCR1B = 0;

    // CTC
    TCCR1B |= (1 << WGM12);

    OCR1A = 249;

    //TIMSK1 |= (1 << OCIE1A);

    TCCR1B |= (1 << CS11) | (1 << CS10);
    
    TIMSK1 &= ~(1 << OCIE1A);
}

static void delay_ms_func(uint16_t ms)
{
    while (ms--)
        _delay_ms(1); 
}

static float midi_to_freq(uint8_t midi) {
    if (midi > 127 || midi <= 0) return 0;
    // midi back to freq
    return 440.0 * pow(2.0, ((float)midi - 69.0) / 12.0);  
}

// calculate top values
static uint16_t freq_to_top(float freq) {
    if (freq <= 0) return 0;
    float top = (F_CPU / (1024.0f * freq)) - 1.0f;
    return (uint16_t)top;
}

//void play_midi(uint8_t midi, uint16_t duration_ms) {
//    float freq = midi_to_freq(midi);
//    
//    if (freq < 1) {
//        OCR0A = 0;               // not music
//        delay_ms_func(duration_ms);
//        return;
//    }
//
//    OCR0A = freq_to_top(freq);
//    OCR0B = OCR0A / 2;   // 50% duty
//
//    delay_ms_func(duration_ms);
//}

uint8_t note_to_block(uint8_t midi)
{
    float range = (float)(max_MIDI - min_MIDI);
    float step_size = range / (float)NUM_BLOCK; // 90 / 9 = 10

    float normalized = (float)(midi - min_MIDI) / step_size; // 100 / 10 = 10
    uint8_t block = (uint8_t)normalized; // = 10 flooring

    if (block >= NUM_BLOCK) block = NUM_BLOCK - 1; // 8

    return block; // 0 - 8
}

// interrupt every 1 ms, flash LED, detect step (read ADC), play music, calculate score

// LED 0.5s 30 - 15 - 7 - 3 - 1 - 0 only the first 10, middle 10, or the last 10 light up

// ADC > 900 < 800 

// LED as long as stepped

// detect whether stepped, speaker duration

// mux 8 - 9

// LCD TIMER start counting duration 1/8 +10 SPI

uint8_t block = 0;

ISR(TIMER1_COMPA_vect)
{
    if (remaining_ms > 0) {
        remaining_ms--;
    }

    adc_val = adc_read();
    //printf("ADC_VAL: %d\n", adc_val);
    printf("BLOCK(ISR): %u\n", block);
    _delay_us(50);

    //ADCSRA |= (1 << ADSC);
    
    if (adc_val >= 850) {
        OCR0B = 0;
    } else {
        
        if (OCR0A != 0) {
            OCR0B = OCR0A / 2; // only when stepped
        } else {
            OCR0B = 0;
        }
    }
}

void play_midi(uint8_t midi, uint16_t duration_ms)
{
    block = note_to_block(midi);
    adc_switch_channel(block);
//    printf("BLOCK(PLAY_MIDI): %u\n", block);
    _delay_us(50);
    
    TIMSK1 |= (1 << OCIE1A);
    
    float freq = midi_to_freq(midi);

    if (freq < 1.0f) {

        OCR0A = 0;
        OCR0B = 0;
        remaining_ms = duration_ms;
        while (remaining_ms > 0);
        return;
    }

    OCR0A = freq_to_top(freq);

    remaining_ms = duration_ms;
    
    while (remaining_ms > 0);
    
    TIMSK1 &= ~(1 << OCIE1A);
}

int main(void)
{
    gpio_init();
    uart_init(); 
    buzzer_init();
    adc_init();
    timer1_init(); // one timer, interrupt every 1 ms, flash LED, detect step (read ADC), play music, calculate score

    sei();

    printf("Mario melody with force sensor mute started.\r\n");

    while (1) {
        for (uint16_t i = 0; i < melody_length; i++) {
            play_midi(melody[i], duration[i]); // play mario
        }
        delay_ms_func(50000);
    }
}
