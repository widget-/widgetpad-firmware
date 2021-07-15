#ifndef WP_PAD
#define WP_PAD

#include "Arduino.h"
#include "stdint.h"

#include "config.h"

uint32_t smooth_input1[NUM_STEPS][ANALOGREAD_SMOOTHING_SAMPLES];
uint32_t smooth_input2[NUM_STEPS][ANALOGREAD_SMOOTHING_SAMPLES];
uint32_t smooth_input3[NUM_STEPS][ANALOGREAD_SMOOTHING_SAMPLES];
uint8_t smooth_input_position = 0;

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

void setupSteps() {
  Joystick.useManualSend(true);
}

void tickSteps() {
  for (uint8_t step = 0; step < NUM_STEPS; step++) {
    PANELS[step].value = handleInput(step, analogRead(PANELS[step].teensyPin));
    PANELS[step].pressed = (PANELS[step].value > PANELS[step].threshold);
    Joystick.button(PANELS[step].gamepadButton, PANELS[step].pressed);
  }
  smooth_input_position = (smooth_input_position + 1) % ANALOGREAD_SMOOTHING_SAMPLES;
  Joystick.send_now();
}

#endif
