/*
 * Copyright (c) 2021, Widget <https://github.com/widget->
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2. See the
 * included LICENSE file.
*/

#ifndef WP_CONFIG
#define WP_CONFIG

#include <cstdint>

#include "Arduino.h"

// #define JOYSTICK_SIZE 64 // override Teensy joystick

#define SENSORS_PER_PANEL 2
#define ANALOGREAD_SMOOTHING_SAMPLES 10 // approx 0.5ms latency

struct Sensor {
    uint32_t threshold;
    uint8_t teensyPin;
    uint32_t value;
    bool pressed;
    uint32_t smoothedSensorData[3][ANALOGREAD_SMOOTHING_SAMPLES]{};
};

struct Panel {
    uint8_t stepOrder;
    uint8_t gamepadButton;
    uint8_t statusLedPin;
    uint8_t ledOrder;
    bool pressed;
    Sensor sensors[SENSORS_PER_PANEL];
};

struct DigitalButton {
    uint8_t gamepadButton;
    uint8_t teensyPin;
    bool pressed;
};

// Default panel configuration

// Be sure to set SENSORS_PER_PANEL up above with the correct
// number of sensors per panel

Panel PANELS[] = {
    { // Left arrow
        3,     // stepOrder
        1,     // gamepadButton
        20,    // statusLedPin
        3,     // ledOrder
        false, // pressed
        {      // sensors
            {
                30, // threshold
                A0   // teensyPin
            }, {
                30, // threshold
                A1   // teensyPin
            }
        }
    }, { // Up arrow
        0,     // stepOrder
        2,     // gamepadButton
        21,    // statusLedPin
        0,     // ledOrder
        false, // pressed
        {      // sensors
            {
                30, // threshold
                A2   // teensyPin
            }, {
                30, // threshold
                A3   // teensyPin
            }
        }
    }, { // Down arrow
        2,     // stepOrder
        3,     // gamepadButton
        22,    // statusLedPin
        1,     // ledOrder
        false, // pressed
        {      // sensors
            {
                100, // threshold
                A4   // teensyPin
            }, {
                100, // threshold
                A5   // teensyPin
            }
        }
    }, { // Right arrow
        1,     // stepOrder
        4,     // gamepadButton
        23,    // statusLedPin
        2,     // ledOrder
        false, // pressed
        {      // sensors
            {
                30, // threshold
                A6   // teensyPin
            }, {
                30, // threshold
                A7   // teensyPin
            }
        }
    }, 
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


// Array length macro shamelessly stolen from https://stackoverflow.com/a/4415646
#define ARRAY_LENGTH(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))


// LEDs are not supported yet:
// #define LED_PIN 10
// #define LEDS_PER_PAD 32
// #define LED_COUNT LEDS_PER_PAD*len(PANELS)
#define SERIAL_BAUD_RATE 2000000

#define NUMBER_OF_PANELS (ARRAY_LENGTH(PANELS))
#define NUMBER_OF_BUTTONS (ARRAY_LENGTH(BUTTONS))
#define INVERT_BUTTON_LOGIC true // true if buttons connect to ground, false if to +VCC

// #define SHOW_STATUS_LEDS // uncomment if status LEDs are wired up

#define PANEL_THRESHOLD_LIFT_GAP 10

#define SPEAKER // uncomment if you have a speaker or buzzer wired up
#define SPEAKER_PIN 3
#define SPEAKER_CLICK_LENGTH 1
#define SPEAKER_BUZZER // uncomment if using a speaker instead of a buzzer
#define SPEAKER_FREQUENCY 4000 // for non-buzzer speakers

#endif
