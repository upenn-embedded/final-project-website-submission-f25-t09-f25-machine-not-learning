#include "LED.h"

void init_strips(void) {
    STRIP0_DDR |= (1 << STRIP0_PIN);
    STRIP1_DDR |= (1 << STRIP1_PIN);
    STRIP2_DDR |= (1 << STRIP2_PIN);
    STRIP3_DDR |= (1 << STRIP3_PIN);

    STRIP_LOW(STRIP0_PORT, STRIP0_PIN);
    STRIP_LOW(STRIP1_PORT, STRIP1_PIN);
    STRIP_LOW(STRIP2_PORT, STRIP2_PIN);
    STRIP_LOW(STRIP3_PORT, STRIP3_PIN);
}

void clear_all_strips(void) {
    for (uint16_t i = 0; i < LED_NUM; i++) SEND_LED0(0,0,0,0);
    RESET_STRIP0();
    for (uint16_t i = 0; i < LED_NUM; i++) SEND_LED1(0,0,0,0);
    RESET_STRIP1();
    for (uint16_t i = 0; i < LED_NUM; i++) SEND_LED2(0,0,0,0);
    RESET_STRIP2();
    for (uint16_t i = 0; i < LED_NUM; i++) SEND_LED3(0,0,0,0);
    RESET_STRIP3();
}

void led_set_strip_segment(uint8_t strip, uint8_t segment) {
    uint16_t start = (segment * LED_NUM) / 3;
    uint16_t end   = ((segment + 1) * LED_NUM) / 3;

    for (uint16_t i = 0; i < start; i++) {
        switch (strip) {
            case 0: SEND_LED0(0,0,0,0); break;
            case 1: SEND_LED1(0,0,0,0); break;
            case 2: SEND_LED2(0,0,0,0); break;
            case 3: SEND_LED3(0,0,0,0); break;
        }
    }
    for (uint16_t i = start; i < end; i++) {
        switch (strip) {
            case 0: SEND_LED0(255,0,0,0); break;
            case 1: SEND_LED1(255,0,0,0); break;
            case 2: SEND_LED2(255,0,0,0); break;
            case 3: SEND_LED3(255,0,0,0); break;
        }
    }
    for (uint16_t i = end; i < LED_NUM; i++) {
        switch (strip) {
            case 0: SEND_LED0(0,0,0,0); break;
            case 1: SEND_LED1(0,0,0,0); break;
            case 2: SEND_LED2(0,0,0,0); break;
            case 3: SEND_LED3(0,0,0,0); break;
        }
    }

    switch (strip) {
        case 0: RESET_STRIP0(); break;
        case 1: RESET_STRIP1(); break;
        case 2: RESET_STRIP2(); break;
        case 3: RESET_STRIP3(); break;
    }
}

void led_set_strip_segment_hint(uint8_t strip, uint8_t segment) {
    uint16_t start = (segment * LED_NUM) / 3;
    uint16_t end   = ((segment + 1) * LED_NUM) / 3;

    for (uint16_t i = 0; i < start; i++) {
        switch (strip) {
            case 0: SEND_LED0(0,0,0,0); break;
            case 1: SEND_LED1(0,0,0,0); break;
            case 2: SEND_LED2(0,0,0,0); break;
            case 3: SEND_LED3(0,0,0,0); break;
        }
    }
    for (uint16_t i = start; i < end; i++) {
        if (i % 4 == 0) {
            switch (strip) {
                case 0: SEND_LED0(245,245,100,0); break;
                case 1: SEND_LED1(245,245,100,0); break;
                case 2: SEND_LED2(245,245,100,0); break;
                case 3: SEND_LED3(245,245,100,0); break;
            }
        } else {
            switch (strip) {
                case 0: SEND_LED0(0,0,0,0); break;
                case 1: SEND_LED1(0,0,0,0); break;
                case 2: SEND_LED2(0,0,0,0); break;
                case 3: SEND_LED3(0,0,0,0); break;
            }
        }
    }
    
    for (uint16_t i = end; i < LED_NUM; i++) {
        switch (strip) {
            case 0: SEND_LED0(0,0,0,0); break;
            case 1: SEND_LED1(0,0,0,0); break;
            case 2: SEND_LED2(0,0,0,0); break;
            case 3: SEND_LED3(0,0,0,0); break;
        }
    }

    switch (strip) {
        case 0: RESET_STRIP0(); break;
        case 1: RESET_STRIP1(); break;
        case 2: RESET_STRIP2(); break;
        case 3: RESET_STRIP3(); break;
    }
}

void led_set_block(uint8_t block) {
    uint8_t segment = block % 3; // 0,1,2
    // Top (strips 2,3)
    if ((block / 3) == 0) {
        led_set_strip_segment(2, segment);
    } else if ((block / 3) == 1) {
        led_set_strip_segment(2, segment);
        led_set_strip_segment(3, segment);
    } else if ((block / 3) == 2) {
        led_set_strip_segment(3, segment);
    }
    // Side (strips 0,1)
    segment = block / 3;
    if (block % 3 == 0) {
        led_set_strip_segment(0, segment);
    } else if (block % 3 == 1) {
        led_set_strip_segment(0, segment);
        led_set_strip_segment(1, segment);
    } else if (block % 3 == 2) {
        led_set_strip_segment(1, segment);
    }
}

void led_set_block_hint(uint8_t block) {
    uint8_t segment = block % 3; // 0,1,2
    // Top (strips 2,3)
    if ((block / 3) == 0) {
        led_set_strip_segment_hint(2, segment);
    } else if ((block / 3) == 1) {
        led_set_strip_segment_hint(2, segment);
        led_set_strip_segment_hint(3, segment);
    } else if ((block / 3) == 2) {
        led_set_strip_segment_hint(3, segment);
    }
    // Side (strips 0,1)
    segment = block / 3;
    if (block % 3 == 0) {
        led_set_strip_segment_hint(0, segment);
    } else if (block % 3 == 1) {
        led_set_strip_segment_hint(0, segment);
        led_set_strip_segment_hint(1, segment);
    } else if (block % 3 == 2) {
        led_set_strip_segment_hint(1, segment);
    }
}

void led_show_block(uint8_t block) {
    uint8_t sreg = SREG;
    cli();
    clear_all_strips();
    led_set_block(block);
    SREG = sreg;
}

void led_show_block_hint(uint8_t block) {
    uint8_t sreg = SREG;
    cli();
    clear_all_strips();
    led_set_block_hint(block);
    SREG = sreg;
}














