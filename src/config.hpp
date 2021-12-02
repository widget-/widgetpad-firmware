/*
 * Copyright (c) 2021, Widget <https://github.com/widget->
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2. See the
 * included LICENSE file.
*/

#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "Arduino.h"

const uint8_t ANALOGREAD_SMOOTHING_SAMPLES = 10; // approx 0.5ms latency

struct Sensor {
    uint32_t threshold = 0;
    uint8_t teensyPin = 0;
    uint32_t value = 0;
    bool pressed = false;
    uint32_t smoothedSensorData[3][ANALOGREAD_SMOOTHING_SAMPLES]{};
};

struct Panel {
    uint8_t stepOrder;
    uint8_t gamepadButton;
    uint8_t statusLedPin;
    uint8_t ledOrder;
    bool pressed;
    std::vector<Sensor> sensors;
};

struct DigitalButton {
    uint8_t gamepadButton;
    uint8_t teensyPin;
    bool pressed;
};

// Default panel configuration

std::unique_ptr<std::vector<Panel>> PANELS;
std::unique_ptr<std::vector<DigitalButton>> BUTTONS;

void loadDefaults() {
    PANELS = std::make_unique<std::vector<Panel>>(std::vector<Panel> {
        { // Left arrow
            3,     // stepOrder
            1,     // gamepadButton
            20,    // statusLedPin
            3,     // ledOrder
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
        }, { // Up arrow
            0,     // stepOrder
            2,     // gamepadButton
            21,    // statusLedPin
            0,     // ledOrder
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
        }, { // Down arrow
            2,     // stepOrder
            3,     // gamepadButton
            22,    // statusLedPin
            1,     // ledOrder
            false, // pressed
            {      // sensors
                {
                    100, // threshold
                    A2   // teensyPin
                }, {
                    100, // threshold
                    A3   // teensyPin
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
                    A4   // teensyPin
                }, {
                    30, // threshold
                    A5   // teensyPin
                }
            }
        },
    });

    BUTTONS = std::make_unique<std::vector<DigitalButton>>( std::vector<DigitalButton> {
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
    });
}


// LEDs are not supported yet:
// #define LED_PIN 10
// #define LEDS_PER_PAD 32
// #define LED_COUNT LEDS_PER_PAD*len(PANELS)
const uint32_t SERIAL_BAUD_RATE = 2000000;

bool INVERT_BUTTON_LOGIC = true; // true if buttons connect to ground, false if to +VCC

// #define SHOW_STATUS_LEDS // uncomment if status LEDs are wired up

uint32_t PANEL_THRESHOLD_LIFT_GAP = 10;

bool SPEAKER = true; // if you have a speaker or buzzer wired up
uint8_t SPEAKER_PIN = 3;
bool SPEAKER_BUZZER = false; // if using a speaker instead of a buzzer
uint32_t SPEAKER_CLICK_LENGTH = 1;
uint32_t SPEAKER_FREQUENCY = 4000; // for non-buzzer speakers
