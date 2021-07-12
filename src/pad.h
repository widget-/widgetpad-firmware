#ifndef WP_PAD
#define WP_PAD

#include "Arduino.h"
#include "stdint.h"

#include "config.h"

void setupSteps() {
  Joystick.useManualSend(true);
}

void tickSteps() {
  for (uint8_t step = 0; step < NUM_STEPS; step++) {
    PANELS[step].value = analogRead(PANELS[step].teensyPin);
    PANELS[step].pressed = (PANELS[step].value > PANELS[step].threshold);
    Joystick.button(PANELS[step].gamepadButton, PANELS[step].pressed);
  }
  Joystick.send_now();
}

#endif
