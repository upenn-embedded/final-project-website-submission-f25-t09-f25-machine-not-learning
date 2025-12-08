#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stdbool.h>

#define ADC_PIN     PE3
#define MUX_S0_PIN  PD2
#define MUX_S1_PIN  PE0
#define MUX_S2_PIN  PE1

extern volatile uint16_t adc_value;
extern volatile bool adc_ready;

void pressure_mux_init(void);
void ADC_init(void);
// int16_t ADC_read(uint8_t block);
void ADC_set_channel(uint8_t block);
uint16_t ADC_sample_block(uint8_t block);

#endif // ADC_H