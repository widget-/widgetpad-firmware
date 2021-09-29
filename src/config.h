/*
 * Copyright (c) 2021, Widget <https://github.com/widget->
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2. See the
 * included LICENSE file.
*/

#ifndef WP_CONFIG
#define WP_CONFIG

// #define JOYSTICK_SIZE 64 // override Teensy joystick

typedef struct Panel Panel;
struct Panel {
    uint32_t threshold;
    uint8_t stepOrder;
    uint8_t gamepadButton;
    uint8_t teensyPin;
    uint8_t statusLedPin;
    uint8_t ledOrder;
    uint32_t value;
    bool pressed;
};
typedef struct DigitalButton DigitalButton;
struct DigitalButton {
    uint8_t gamepadButton;
    uint8_t teensyPin;
    bool pressed;
};

// Default panel configuration

Panel PANELS[] = {
    { // Left arrow
        150, // threshold
        3,   // stepOrder
        1,   // gamepadButton
        A0,  // teensyPin
        20,  // statusLedPin
        3    // ledOrder
    }, { // Up arrow
        150, // threshold
        0,   // stepOrder
        2,   // gamepadButton
        A1,  // teensyPin
        21,  // statusLedPin
        0    // ledOrder
    }, { // Down arrow
        150, // threshold
        2,   // stepOrder
        3,   // gamepadButton
        A2,  // teensyPin
        22,  // statusLedPin
        2    // ledOrder
    }, { // Right arrow
        150, // threshold
        1,   // stepOrder
        4,   // gamepadButton
        A3,  // teensyPin
        23,  // statusLedPin
        1    // ledOrder
    }
};

DigitalButton BUTTONS[] = {
    { // Left
        5,   // gamepadButton
        7,   // teensyPin
    }, { // Right
        6,   // gamepadButton
        6,   // teensyPin
    }, { // Select
        7,   // gamepadButton
        5,   // teensyPin
    }, { // Back
        8,   // gamepadButton
        4,   // teensyPin
    }
};

#define LED_PIN 10
#define LEDS_PER_PAD 32
#define SERIAL_BAUD_RATE 2000000

#define LED_COUNT LEDS_PER_PAD*len(PANELS)
#define NUMBER_OF_PANELS (sizeof PANELS / sizeof PANELS[0])
#define NUMBER_OF_BUTTONS (sizeof BUTTONS / sizeof BUTTONS[0])
#define INVERT_BUTTON_LOGIC true // true if buttons connect to ground, false if to +VCC

#define SHOW_STATUS_LEDS true

#define ANALOGREAD_SMOOTHING_SAMPLES 10 // approx 0.5ms latency
#define PANEL_THRESHOLD_LIFT_GAP 10

#define SPEAKER true
#define SPEAKER_PIN 3
#define SPEAKER_CLICK_LENGTH 1
#define SPEAKER_BUZZER true // if speaker is a buzzer
#define SPEAKER_FREQUENCY 4000 // for non-buzzer speakers

#endif
