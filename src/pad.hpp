/*
 * Copyright (c) 2021, Widget <https://github.com/widget->
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2. See the
 * included LICENSE file.
*/

#ifndef WP_PAD
#define WP_PAD

#include <cstdint>

#include "Arduino.h"

#include "config.hpp"
#include "serial.hpp"
// #include "status_led.h"

// uint32_t smooth_input1[NUMBER_OF_PANELS][SENSORS_PER_PANEL][ANALOGREAD_SMOOTHING_SAMPLES] = {};
// uint32_t smooth_input2[NUMBER_OF_PANELS][SENSORS_PER_PANEL][ANALOGREAD_SMOOTHING_SAMPLES] = {};
// uint32_t smooth_input3[NUMBER_OF_PANELS][SENSORS_PER_PANEL][ANALOGREAD_SMOOTHING_SAMPLES] = {};
uint8_t smooth_input_position = 0;

#ifdef SPEAKER
  bool speakerOn = false;
  uint32_t speakerTickEnd = 0;
#endif

bool sendJoystickUpdate = false;

uint32_t smooth(uint32_t samples[]) {
  uint32_t sum = 0;

  for (uint8_t i = 0; i < ANALOGREAD_SMOOTHING_SAMPLES; i++) {
    sum += samples[(i + smooth_input_position) % ANALOGREAD_SMOOTHING_SAMPLES];
  }

  return sum / ANALOGREAD_SMOOTHING_SAMPLES;
}

// uint32_t handleInput(uint8_t panel, uint8_t sensor, uint32_t value) {
//   smooth_input1[panel][sensor][smooth_input_position] = value;
//   smooth_input2[panel][sensor][smooth_input_position] = smooth(smooth_input1[panel][sensor]);
//   smooth_input3[panel][sensor][smooth_input_position] = smooth(smooth_input2[panel][sensor]);
//   return smooth_input3[panel][sensor][smooth_input_position];
// }

uint32_t handleInput(Sensor &sensor, uint32_t value) {
  sensor.smoothedSensorData[0][smooth_input_position] = value;
  sensor.smoothedSensorData[1][smooth_input_position] = smooth(sensor.smoothedSensorData[0]);
  sensor.smoothedSensorData[2][smooth_input_position] = smooth(sensor.smoothedSensorData[1]);

  // for (int i = 0; i < 3; i++) {
  //   Serial.print("Smoothed data ");
  //   Serial.print(i);
  //   Serial.print(": ");
  //   for (uint32_t e : sensor.smoothedSensorData[i]) {
  //     Serial.print(e);
  //     Serial.print(", ");
  //   }
  //   Serial.println("");
  // }

  return sensor.smoothedSensorData[2][smooth_input_position];
}

void setJoystickButton(uint8_t button, bool pressed) {
    Joystick.button(button, pressed);
    sendJoystickUpdate = true;
}

void setupSteps() {
  Joystick.useManualSend(true);
  
  for (Panel panel : PANELS) {
    for (Sensor sensor : panel.sensors) {
      pinMode(sensor.teensyPin, INPUT);
    }
  }

  for (DigitalButton button : BUTTONS) {
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


  for (Panel &panel : PANELS) {
  // for (uint8_t panelIndex = 0; panelIndex < NUMBER_OF_PANELS; panelIndex++) {
  //   Panel panel = PANELS[panelIndex];
    bool pressed = false;
    for (Sensor &sensor : panel.sensors) {
    // for (int sensorIndex = 0; sensorIndex < SENSORS_PER_PANEL; sensorIndex++) {
    //   Sensor sensor = panel.sensors[sensorIndex];
      // sensor.value = handleInput(panelIndex, sensorIndex, analogRead(sensor.teensyPin));
      // Serial.print("Value for sensor at teensy pin ");
      // Serial.print(sensor.teensyPin);
      // Serial.print(" is ");
      // Serial.println(analogRead(sensor.teensyPin));
      sensor.value = handleInput(sensor, analogRead(sensor.teensyPin));
      uint32_t threshold = sensor.threshold - (panel.pressed ? PANEL_THRESHOLD_LIFT_GAP : 0);

      pressed = pressed || (sensor.value > threshold);
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
  for (DigitalButton &button : BUTTONS) {
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

  // serialDebugPanels();
  // delay(1000);
  printValues();

  delay(1); // limit framerate to ~1000Hz
}

#endif
