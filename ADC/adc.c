#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "mario_melody.h"
#include "adc.h"

// // pressure sensor init
// void pressure_mux_init(void) {
//     DDRB |= (1 << MUX_S0_PIN); // output
//     DDRD |= (1 << MUX_S1_PIN)|(1 << MUX_S2_PIN);
//     PORTB &= ~(1 << MUX_S0_PIN);
//     PORTD &= ~(1 << MUX_S1_PIN);
//     PORTD &= ~(1 << MUX_S2_PIN);
// }

// void ADC_init(void) {
//     PRR0 &= ~(1 << PRADC); // clear power reduction bit for ADC

//     // Vref = AVcc
//     ADMUX = (1 << REFS0);
    
//     // Prescale 128 -> 125kHz
//     ADCSRA = (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

//     // Disable auto trigger
//     ADCSRA &= ~(1 << ADATE);

//     // Enable ADC
//     ADCSRA |= (1 << ADEN);
// }


// // ----------- read ADC --------------
// //
// int16_t ADC_read(uint8_t block)
// {
//     if (block >= NUM_BLOCK) block = NUM_BLOCK - 1;

//     if (block <= 6) {
//         PORTB &= ~(1 << MUX_S0_PIN);
//         ADMUX = (ADMUX & 0xF0) | block;
//     } else {
//         uint8_t sel = block - 7;
//         if (sel == 1) PORTB |=  (1 << MUX_S0_PIN);
//         else          PORTB &= ~(1 << MUX_S0_PIN);
//         ADMUX = (ADMUX & 0xF0) | 7;
//     }

//     _delay_us(100); // mux + admux settle

//     ADCSRA |= (1 << ADSC);
//     while (ADCSRA & (1 << ADSC));

//     return ADC;
// }

volatile uint16_t adc_value = 0;
volatile bool adc_ready = false;

void pressure_mux_init(void)
{
    // multiplexer select pins
    DDRD |= (1 << MUX_S0_PIN);
    DDRE |= (1 << MUX_S1_PIN) | (1 << MUX_S2_PIN);

    PORTD &= ~(1 << MUX_S0_PIN);
    PORTE &= ~(1 << MUX_S1_PIN);
    PORTE &= ~(1 << MUX_S2_PIN);
}

void ADC_init(void)
{
    PRR0 &= ~(1 << PRADC);

    // AVcc reference
    ADMUX = (1 << REFS0);

    // Prescaler 128 for 125kHz ADC clock
    ADCSRA = (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

    // Enable ADC + interrupt
    ADCSRA |= (1<<ADEN) | (1<<ADIE);
}

void ADC_set_channel(uint8_t block)
{
    if (block >= NUM_BLOCK) block = NUM_BLOCK - 1;

    if (block <= 6) {
        PORTD &= ~(1 << MUX_S0_PIN);
        ADMUX = (ADMUX & 0xF0) | block;
    } else {
        uint8_t sel = block - 7;
        if (sel == 1)
            PORTD |= (1 << MUX_S0_PIN);
        else
            PORTD &= ~(1 << MUX_S0_PIN);

        ADMUX = (ADMUX & 0xF0) | 7;
    }
    _delay_us(20);
}

uint16_t ADC_sample_block(uint8_t block)
{
    // set block channel, but not start conversion yet
    ADC_set_channel(block);

    // return the last real ADC value filled by ISR
    return adc_value;
}

// ADC conversion complete interrupt
ISR(ADC_vect)
{
    adc_value = ADC;
    adc_ready = true;
}