/*
 * Copyright (c) 2021, Widget <https://github.com/widget->
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2. See the
 * included LICENSE file.
*/

#ifndef WP_STATUS_LED
#define WP_STATUS_LED

#include "Arduino.h"
#include "stdint.h"
#include "EEPROM.h"

#include "config.hpp"

// bool STATUS_LEDS[NUMBER_OF_PANELS];

void setup_status_leds() {
  // for (uint8_t step = 0; step < NUMBER_OF_PANELS; step++) {
  //   pinMode(PANELS[step].statusLedPin, OUTPUT);
  // }
}

void update_status_led(uint8_t panel, bool on) {
  // STATUS_LEDS[panel] = on;
  // digitalWrite(PANELS[panel].statusLedPin, STATUS_LEDS[panel] ? HIGH : LOW);
}

#endif