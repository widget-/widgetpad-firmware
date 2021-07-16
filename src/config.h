/*
 * Copyright (c) 2021, Widget <https://github.com/widget->
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2. See the
 * included LICENSE file.
*/

#ifndef WP_CONFIG
#define WP_CONFIG

typedef struct Panel Panel;
struct Panel {
    uint32_t threshold;
    uint8_t stepOrder;
    uint8_t gamepadButton;
    uint8_t teensyPin;
    uint8_t ledOrder;
    uint32_t value;
    bool pressed;
};

// Default panel configuration

Panel PANELS[] = {
    { // Left arrow
        150, // threshold
        3,   // stepOrder
        1,   // gamepadButton
        A3,  // teensyPin
        3    // ledOrder
    }, { // Up arrow
        150, // threshold
        0,   // stepOrder
        2,   // gamepadButton
        A2,  // teensyPin
        0    // ledOrder
    }, { // Down arrow
        150, // threshold
        2,   // stepOrder
        3,   // gamepadButton
        A1,  // teensyPin
        2    // ledOrder
    }, { // Right arrow
        150, // threshold
        1,   // stepOrder
        4,   // gamepadButton
        A0,  // teensyPin
        1    // ledOrder
    }
};

#define LED_PIN 10
#define LEDS_PER_PAD 32
#define SERIAL_BAUD_RATE 2000000


#define LED_COUNT LEDS_PER_PAD*len(PANELS)
#define NUMBER_OF_PANELS (sizeof PANELS / sizeof PANELS[0])

#define ANALOGREAD_SMOOTHING_SAMPLES 16 // approx 0.5ms latency

#endif
