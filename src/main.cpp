#include "Arduino.h"
#include "stdint.h"

#include "config.h"

#include "serial.h"
#include "pad.h"

void setup() {
  setupSteps();
  setupSerial();
}

void loop() {
  tickSteps();
  tickSerial();
}
