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
