/*
* File:   force_sensor.c
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

#include <stdlib.h>
#include "uart.h"

#define PIN0 0
#define PIN1 1
#define PIN2 2

void gpio_init(){
    
    DDRB |= (1 << PIN0)|(1 << PIN1)|(1 << PIN2); // output
    // 0 0 1, yellow
    PORTB |= (1 << PIN0);
    PORTB &= ~(1 << PIN1);
    PORTB &= ~(1 << PIN2);
    
}

void adc_init(){
    DDRC &= ~(1 << PIN0); // set PC0 as an input

    PRR0 &= ~(1 << PRADC); // clear the ADC power reduction bit by writing a logic zero to it in the power reduction register, this turns the ADC on

    ADMUX |= (1 << REFS0); // select the reference voltage
    ADMUX &= ~(1 << REFS1);
    
    ADCSRA |= (1 << ADPS0);// set up the ADC clock (prescalers), must be within 50kHz to 200kHz
    ADCSRA |= (1 << ADPS1);
    ADCSRA |= (1 << ADPS2);

    ADMUX &= ~(1 << MUX0); // select the ADC channel to read
    ADMUX &= ~(1 << MUX1);
    ADMUX &= ~(1 << MUX2);
    ADMUX &= ~(1 << MUX3);

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

void main(void) {

    uart_init();
    adc_init();
    gpio_init();

    while (1)
        {
            printf("%u\n",adc_read());
            _delay_ms(500);
        } 
    
}

// LED 0.5s 30 - 15 - 7 - 3 - 1 - 0

// ADC > 850 < 800 =

// LED as long as stepped

// speaker duration

// mux 8 - 9

// LCD TIMER start counting duration 1/8 +10 SPI


