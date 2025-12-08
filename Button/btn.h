#ifndef BTN_H
#define BTN_H

#include <stdbool.h>

#define BTN_PIN             PD7
#define PIN_BUTTON_PIN      PIND
#define DDR_BUTTON_PIN      DDRD
#define PORT_BUTTON_PIN     PORTD

void btn_init();
bool btn_is_pressed();

#endif // BTN_H