/*
 * Copyright (c) 2021, Widget <https://github.com/widget->
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2. See the
 * included LICENSE file.
*/

#include "Arduino.h"
#include <cstdint>

#include "config.hpp"

#include "serial.hpp"
#include "pad.hpp"
#include "eeprom_save.hpp"

void setup() {
  loadDefaults();
  setupEeprom();
  setupSteps();
  setupSerial();
}

void loop() {
  tickSteps();
  tickSerial();
}
