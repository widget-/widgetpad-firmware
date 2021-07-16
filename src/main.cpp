/*
 * Copyright (c) 2021, Widget <https://github.com/widget->
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2. See the
 * included LICENSE file.
*/

#include "Arduino.h"
#include "stdint.h"

#include "config.h"

#include "serial.h"
#include "pad.h"
#include "eeprom_save.h"

void setup() {
  setupEeprom();
  setupSteps();
  setupSerial();
}

void loop() {
  tickSteps();
  tickSerial();
}
