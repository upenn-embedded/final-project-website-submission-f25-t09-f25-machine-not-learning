#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "btn.h"

void btn_init(){ 
    // GPIO Pins Setup
    DDR_BUTTON_PIN &= ~(1 << BTN_PIN); // button as input
    PORT_BUTTON_PIN |= (1 << BTN_PIN); // enable pull-up
}

bool btn_is_pressed(){
    if (!(PIN_BUTTON_PIN & (1 << BTN_PIN))) {
        _delay_ms(10); // debounce
        if (!(PIN_BUTTON_PIN & (1 << BTN_PIN))) {
            return true;
        }
    }
    return false;
}