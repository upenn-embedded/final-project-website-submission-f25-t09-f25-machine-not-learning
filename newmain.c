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

//#include "uart.h"
#include "mario_melody.h"
#include "adc.h"
#include "ST7735.h"
#include "LCD_GFX.h"
#include "LED.h"
#include "uart_sync_master.h"
#include "btn.h"

volatile int16_t remaining_ms = 0;
volatile uint8_t current_midi; 
volatile bool get_pressure = false;
volatile bool sample_flag = false;
volatile bool get_point = false;
volatile uint16_t score = 0;
volatile char score_text[16] = {0};
volatile bool first_note_signal = true;

// ----------- timer1 for delay ms --------------

void timer1_init(void)
{
    TCCR1A = 0;
    TCCR1B = 0;

    // CTC
    TCCR1B |= (1 << WGM12);
    OCR1A = 1245;
//    OCR1A = 249;
}

void timer1_start(void)
{
    TCNT1 = 0;
    TCCR1B |= (1 << CS11) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
}

void timer1_stop(void)
{
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
    TIMSK1 &= ~(1 << OCIE1A);
}

// ISR(TIMER1_COMPA_vect) {
//     sample_flag = true;
// }

ISR(TIMER1_COMPA_vect)
{
    sample_flag = true;
    adc_ready = false;
    ADCSRA |= (1 << ADSC); // start ADC conversion
}


// ----------- main --------------
void play_one_note(uint8_t midi, uint16_t dur_ms) {
    if (midi == 0) {
        delay_ms_func(dur_ms);
        stop_buzzer_sound();
        return;
    }

    uint16_t top   = freq_to_top(midi_to_freq(midi));
    uint8_t  block = note_to_block(midi);

    uart_send_block_hint(block);

    if (first_note_signal) {
        first_note_signal = false;
        _delay_ms(400);
    }

    uint16_t ticks = dur_ms / 5;
    //uint16_t ticks = dur_ms;
    if (ticks == 0) ticks = 1; 

    timer1_start();
    sample_flag = false;

    while (ticks > 0) {
        if (sample_flag) {
            sample_flag = false;
            ticks--;
            // int16_t adc_val = ADC_read(block); 
            uint16_t adc_val = ADC_sample_block(block);

            if (adc_val < 850) {
                uart_send_block_solid(block);
                play_midi_top(top);
                get_point = true;
            } else {
                uart_send_block_hint(block);
                stop_buzzer_sound();
            }
        }
    }

    timer1_stop();
    stop_buzzer_sound();
}

void play_music(void) {
    first_note_signal = true;
    score = 0;
    LCD_setScreen(WHITE);

    for (uint16_t i = 0; i < melody_length; i++) 
    {
        uint8_t  midi   = melody[i];
        uint16_t dur_ms = duration[i];

        play_one_note(midi, dur_ms);

        if (get_point) {
            get_point = false;
            score += 1;
        }

        sprintf(score_text, "Score: %d", score);
        LCD_drawStringFast(60, 60, score_text, BLACK, WHITE);
    }
}

int main(void) {
    cli();
    uart_init_master();
    pressure_mux_init();
    ADC_init();
    buzzer_init();
    lcd_init();
    timer1_init();
    btn_init();
    sei();
    
    LCD_setScreen(WHITE);

    while(1){
        if (btn_is_pressed()) {     
            _delay_ms(10);          
            while (btn_is_pressed()); 
            play_music();
        }
    }
}