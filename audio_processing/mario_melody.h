#ifndef MARIO_MELOTY_H
#define MARIO_MELOTY_H

#define PWM_PIN  PD5
#define NUM_BLOCK   9 

extern const uint8_t melody[];
extern const uint16_t duration[];
extern const uint16_t melody_length;
extern const uint8_t min_MIDI;
extern const uint8_t max_MIDI;

void buzzer_init(void);
void delay_ms_func(uint16_t ms);
float midi_to_freq(uint8_t midi);
uint16_t freq_to_top(float freq);
void play_midi_top(uint16_t top);
void play_midi_2(uint8_t midi, uint16_t duration_ms);
void play_midi(uint8_t midi);
void stop_buzzer_sound(void);
uint8_t note_to_block(uint8_t midi);


#endif // MARIO_MELOTY_H
