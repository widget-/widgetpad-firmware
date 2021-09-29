/*
 * Copyright (c) 2021, Widget <https://github.com/widget->
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2. See the
 * included LICENSE file.
*/

#ifndef WP_SERIAL
#define WP_SERIAL

#include "Arduino.h"
#include "stdint.h"

#include "config.h"
#include "eeprom_save.h"

#define BUFFER_SIZE 256
char read_buffer[BUFFER_SIZE];
char write_buffer[BUFFER_SIZE];

uint32_t frames = 0;

void setupSerial() {
  Serial.begin(SERIAL_BAUD_RATE);
}

void printFps() {
  sprintf(write_buffer, "fps %f", (float)frames*1000.0/(float)millis());
  Serial.println(write_buffer);
}

void printThresholds() {
  Serial.print("t ");
  for (uint8_t panel = 0; panel < NUMBER_OF_PANELS; panel++) {
    Serial.print(PANELS[panel].threshold);
    Serial.print(' ');
  }
  Serial.print('\n');
}

void printButtons() {
  Serial.print("b ");
  for (uint8_t button = 0; button < NUMBER_OF_BUTTONS; button++) {
    Serial.print(BUTTONS[button].pressed ? "1" : "0");
    Serial.print(' ');
  }
  Serial.print('\n');
}

void printValues() {
  Serial.print("v ");
  for (uint8_t panel = 0; panel < NUMBER_OF_PANELS; panel++) {
    Serial.print(PANELS[panel].value);
    Serial.print(' ');
  }
  Serial.print('\n');
}

void saveToEeprom() {
  storeEeprom();
  Serial.print("s ok\n");
}

void loadFromEeprom() {
  if (loadEeprom()) {
    Serial.print("l ok\n");
  } else {
    Serial.print("l failed\n");
  }
  printThresholds();
}

void resetEeprom() {
  clearEeprom();
  Serial.print("c ok\n");
}

void writeThreshold(size_t bytes_read) {
  if (bytes_read < 3) return;

  uint8_t index = read_buffer[2] - '0';

  PANELS[index].threshold = atoi(read_buffer + 3);
}

void tickSerial() {
  frames++;

  while (Serial.available() > 0) {
    size_t bytesRead = Serial.readBytesUntil('\n', read_buffer, BUFFER_SIZE - 1);
    read_buffer[bytesRead] = '\0';

    if (bytesRead == 0) return;

    switch (read_buffer[0]) {
    case 'b':
      printButtons();
      break;
    case 'c':
      resetEeprom();
      break;
    case 'f':
      printFps();
      break;
    case 'l':
      loadFromEeprom();
      break;
    case 's':
      saveToEeprom();
      break;
    case 't':
      printThresholds();
      break;
    case 'v':
      printValues();
      break;
    case 'T':
      writeThreshold(bytesRead);
      break;
    }
  }
}

#endif
