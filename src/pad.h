/*
 * Copyright (c) 2021, Widget <https://github.com/widget->
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2. See the
 * included LICENSE file.
*/

#ifndef WP_PAD
#define WP_PAD

#include "Arduino.h"
#include "stdint.h"

#include "config.h"
#include "status_led.h"

uint32_t smooth_input1[NUMBER_OF_PANELS][ANALOGREAD_SMOOTHING_SAMPLES];
uint32_t smooth_input2[NUMBER_OF_PANELS][ANALOGREAD_SMOOTHING_SAMPLES];
uint32_t smooth_input3[NUMBER_OF_PANELS][ANALOGREAD_SMOOTHING_SAMPLES];
uint8_t smooth_input_position = 0;

bool speakerOn = false;
uint32_t speakerTickEnd = 0;

bool sendJoystickUpdate = false;

uint32_t smooth(uint32_t samples[], uint8_t position) {
  uint32_t sum = 0;

  for (uint8_t i = 0; i < ANALOGREAD_SMOOTHING_SAMPLES; i++) {
    sum += samples[(i + smooth_input_position) % ANALOGREAD_SMOOTHING_SAMPLES];
  }

  return sum / ANALOGREAD_SMOOTHING_SAMPLES;
}

uint32_t handleInput(uint8_t panel, uint32_t value) {
  smooth_input1[panel][smooth_input_position] = value;
  smooth_input2[panel][smooth_input_position] = smooth(smooth_input1[panel], smooth_input_position);
  smooth_input3[panel][smooth_input_position] = smooth(smooth_input2[panel], smooth_input_position);
  return smooth_input3[panel][smooth_input_position];
}

void setJoystickButton(uint8_t button, bool pressed) {
    Joystick.button(button, pressed);
    sendJoystickUpdate = true;
}

void setupSteps() {
  Joystick.useManualSend(true);
  
  for (uint8_t step = 0; step < NUMBER_OF_PANELS; step++) {
    pinMode(PANELS[step].teensyPin, INPUT);
  }

  for (uint8_t button = 0; button < NUMBER_OF_BUTTONS; button++) {
    uint8_t buttonInputMode = INVERT_BUTTON_LOGIC ? INPUT_PULLUP : INPUT_PULLDOWN;
    pinMode(BUTTONS[button].teensyPin, buttonInputMode);
  }

  if (SPEAKER) {
    pinMode(SPEAKER_PIN, OUTPUT);
    if (SPEAKER_BUZZER) {
      analogWriteFrequency(SPEAKER_PIN, SPEAKER_FREQUENCY);
    }
  }

  if (SHOW_STATUS_LEDS) {
    setup_status_leds();
  }
}

void toggleStep(uint8_t step, bool pressed) {
    PANELS[step].pressed = pressed;
    setJoystickButton(PANELS[step].gamepadButton, pressed);

    if (SPEAKER && pressed) {
      speakerOn = true;
    }

    if (SHOW_STATUS_LEDS) {
      update_status_led(step, pressed);
    }
}

void tickSteps() {
  speakerOn = false;

  // panels
  for (uint8_t step = 0; step < NUMBER_OF_PANELS; step++) {
    PANELS[step].value = handleInput(step, analogRead(PANELS[step].teensyPin));
    uint32_t threshold = PANELS[step].threshold - (PANELS[step].pressed ? PANEL_THRESHOLD_LIFT_GAP : 0);
    bool pressed = PANELS[step].value > threshold;
    if (PANELS[step].pressed != pressed) {
      toggleStep(step, pressed);
    }
  }

  if (SPEAKER && speakerOn) {
    if (SPEAKER_BUZZER) {
      digitalWriteFast(SPEAKER_PIN, HIGH);
    } else {
      tone(SPEAKER_PIN, SPEAKER_FREQUENCY);
    }
    speakerTickEnd = millis() + SPEAKER_CLICK_LENGTH;
    speakerOn = false;
  } else if (SPEAKER && speakerTickEnd != 0 && speakerTickEnd < millis()) {
    if (SPEAKER_BUZZER) {
      digitalWriteFast(SPEAKER_PIN, LOW);
    } else {
      noTone(SPEAKER_PIN);
    }
  }

  smooth_input_position = (smooth_input_position + 1) % ANALOGREAD_SMOOTHING_SAMPLES;

  // buttons
  for (uint8_t button = 0; button < NUMBER_OF_BUTTONS; button++) {
    bool pressed = (digitalRead(BUTTONS[button].teensyPin) == 1) ^ INVERT_BUTTON_LOGIC;
    if (pressed != BUTTONS[button].pressed) {
      BUTTONS[button].pressed = pressed;
      setJoystickButton(BUTTONS[button].gamepadButton, pressed);
    }
  }


  if (sendJoystickUpdate) {
    Joystick.send_now();
    sendJoystickUpdate = false;
  }
}

#endif
