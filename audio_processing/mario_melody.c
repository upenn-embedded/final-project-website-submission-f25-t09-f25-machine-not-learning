#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "uart.h"
#include "mario_melody.h"

const uint8_t melody[] = {
 64, 0, 63, 0, 62, 60, 0, 55, 0, 76, 0, 74, 72, 0, 76, 0, 72, 74, 76, 76, 74, 72, 76, 77, 79, 81, 0, 72, 72, 69, 67, 72, 72, 72, 65, 67, 69, 71, 0, 91, 91, 0, 76, 0, 74, 72, 0, 76, 0, 72, 74, 76, 76, 74, 72, 81, 79, 84, 76, 74, 0, 72, 72, 74, 76, 72, 72, 72, 77, 76, 72, 74, 0, 91, 0, 79, 0, 69, 71, 72, 71, 0, 67, 0, 65, 0, 67, 65, 64, 0, 65, 0, 66, 0, 67, 0, 69, 71, 72, 71, 0, 67, 0, 75, 74, 0, 72,};

const uint16_t duration[] = {
    125, 125, 125, 125, 125, 125, 250, 375, 575, 250, 250, 125, 125, 250, 250, 450, 125, 125, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 200, 125, 250, 375, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 125, 125, 250, 250, 450, 125, 125, 250, 250, 250, 250, 250, 250, 250, 125, 125, 250, 250, 250, 250, 250, 125, 250, 375, 250, 250, 250, 250, 250, 125, 250, 125, 450, 250, 250, 250, 250, 125, 250, 375, 250, 250, 125, 375, 125, 125, 125, 125, 125, 125, 125, 375, 250, 250, 250, 250, 125, 250, 375, 250, 250, 250, 250,};

const uint16_t melody_length = 107;

const uint8_t min_MIDI = 55;

const uint8_t max_MIDI = 91;

void buzzer_init(void) {
    DDRD |= (1 << PWM_PIN);
    PORTD &= ~(1 << PWM_PIN);
    
    TCCR0A |= (1 << WGM00);
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << WGM02);
    
    TCCR0A |= (1 << COM0B1);

    TCCR0B |= (1 << CS02) | (1 << CS00);
  
    // initialize top and duty
    OCR0A = 0; 
    OCR0B = 0; // 50% duty cycle
}

void delay_ms_func(uint16_t ms)
{
    while (ms--) _delay_ms(1); 
}

float midi_to_freq(uint8_t midi) {
    if (midi > 127 || midi <= 0) return 0;
    // midi back to freq
    return 440.0 * pow(2.0, ((float)midi - 69.0) / 12.0);  
}

// calculate top values
uint16_t freq_to_top(float freq) {
    if (freq <= 0) return 0;
    float top = (F_CPU / (1024.0f * freq)) - 1.0f;
    return (uint16_t)top;
}

void play_midi_2(uint8_t midi, uint16_t duration_ms) {
    float freq = midi_to_freq(midi);
    
    if (freq < 1) {
        OCR0A = 0;               // not music
        OCR0B = OCR0A / 2;
        delay_ms_func(duration_ms);
        return;
    }

    OCR0A = freq_to_top(freq);
    OCR0B = OCR0A / 2;   // 50% duty

    delay_ms_func(duration_ms);
}

void play_midi_top(uint16_t top) {
    OCR0A = top;
    OCR0B = OCR0A / 2;
}

void play_midi(uint8_t midi) {
    float freq = midi_to_freq(midi);
    
    if (freq < 1) {
        OCR0A = 0;               // not music
        OCR0B = OCR0A / 2;
        return;
    }

    OCR0A = freq_to_top(freq);
    OCR0B = OCR0A / 2;   // 50% duty
}

void stop_buzzer_sound(void) {
    OCR0A = 0;
    OCR0B = 0;
}

uint8_t note_to_block(uint8_t midi)
{
    float range = (float)(max_MIDI - min_MIDI);
    float step_size = range / (float)NUM_BLOCK;

    float normalized = (float)(midi - min_MIDI) / step_size;
    uint8_t block = (uint8_t)normalized; // flooring

    if (block >= NUM_BLOCK) block = NUM_BLOCK - 1;

    return block;
}


// test
// int main (void)
// {
//     initialize();
//     uart_init();
    
//     for (uint16_t idx = 0; idx < melody_length; idx++)
//     {
//         play_midi(melody[idx], duration[idx]);
//     }  

//     while(1);
// }