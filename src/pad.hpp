/*
 * Copyright (c) 2021, Widget <https://github.com/widget->
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2. See the
 * included LICENSE file.
*/

#pragma once

#include <cstdint>

#include "Arduino.h"

#include "config.hpp"
#include "serial.hpp"

uint8_t smooth_input_position = 0;

#ifdef SPEAKER
  bool speakerOn = false;
  uint32_t speakerTickEnd = 0;
#endif

bool sendJoystickUpdate = false;

auto smooth(uint32_t samples[]) -> uint32_t {
  uint32_t sum = 0;

  for (uint8_t i = 0; i < ANALOGREAD_SMOOTHING_SAMPLES; i++) {
    sum += samples[(i + smooth_input_position) % ANALOGREAD_SMOOTHING_SAMPLES];
  }

  return sum / ANALOGREAD_SMOOTHING_SAMPLES;
}

auto handleInput(Sensor &sensor, uint32_t value) -> uint32_t {
  sensor.smoothedSensorData[0][smooth_input_position] = value;
  sensor.smoothedSensorData[1][smooth_input_position] = smooth(sensor.smoothedSensorData[0]);
  sensor.smoothedSensorData[2][smooth_input_position] = smooth(sensor.smoothedSensorData[1]);

  return sensor.smoothedSensorData[2][smooth_input_position];
}

void setJoystickButton(uint8_t button, bool pressed) {
    Joystick.button(button, pressed);
    sendJoystickUpdate = true;
}

void setupSteps() {
  Joystick.useManualSend(true);
  
  for (const Panel &panel : *PANELS) {
    for (Sensor sensor : panel.sensors) {
      pinMode(sensor.teensyPin, INPUT);
    }
  }

  for (DigitalButton button : *BUTTONS) {
    uint8_t buttonInputMode = INVERT_BUTTON_LOGIC ? INPUT_PULLUP : INPUT_PULLDOWN;
    pinMode(button.teensyPin, buttonInputMode);
  }

  #ifdef SPEAKER
    pinMode(SPEAKER_PIN, OUTPUT);
    #ifdef SPEAKER_BUZZER
      analogWriteFrequency(SPEAKER_PIN, SPEAKER_FREQUENCY);
    #endif
  #endif

  #ifdef SHOW_STATUS_LEDS
    setup_status_leds();
  #endif
}

void togglePanel(Panel &panel, bool pressed) {
    panel.pressed = pressed;
    setJoystickButton(panel.gamepadButton, pressed);

    #ifdef SPEAKER
      if (pressed) {
        speakerOn = true;
      }
    #endif

    #ifdef SHOW_STATUS_LEDS
      update_status_led(step, pressed);
    #endif
}


void tickSteps() {
  #ifdef SPEAKER
    speakerOn = false;
  #endif

  // panels
  for (Panel &panel : *PANELS) {
    bool pressed = false;
    for (Sensor &sensor : panel.sensors) {
      sensor.value = handleInput(sensor, analogRead(sensor.teensyPin));
      uint32_t threshold = sensor.threshold - (panel.pressed ? PANEL_THRESHOLD_LIFT_GAP : 0);
      sensor.pressed = sensor.value > threshold;

      pressed = pressed || sensor.pressed;
    }
    if (panel.pressed != pressed) {
      togglePanel(panel, pressed);
    }
  }


  #ifdef SPEAKER
    if (speakerOn) {
      #ifdef SPEAKER_BUZZER
        digitalWriteFast(SPEAKER_PIN, HIGH);
      #else
        tone(SPEAKER_PIN, SPEAKER_FREQUENCY);
      #endif
      speakerTickEnd = millis() + SPEAKER_CLICK_LENGTH;
      speakerOn = false;
    } else if (speakerTickEnd != 0 && speakerTickEnd < millis()) {
      #ifdef SPEAKER_BUZZER
        digitalWriteFast(SPEAKER_PIN, LOW);
      #else
        noTone(SPEAKER_PIN);
      #endif
    }
  #endif

  smooth_input_position = (smooth_input_position + 1) % ANALOGREAD_SMOOTHING_SAMPLES;

  // buttons
  for (DigitalButton &button : *BUTTONS) {
    bool pressed = (digitalRead(button.teensyPin) == 1) ^ INVERT_BUTTON_LOGIC;
    if (pressed != button.pressed) {
      button.pressed = pressed;
      setJoystickButton(button.gamepadButton, pressed);
    }
  }

  if (sendJoystickUpdate) {
    Joystick.send_now();
    sendJoystickUpdate = false;
  }

  delay(1); // limit framerate to ~1000Hz
}
