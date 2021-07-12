#ifndef WP_SERIAL
#define WP_SERIAL

#include "config.h"

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
  for (uint8_t panel = 0; panel < NUM_STEPS; panel++) {
    Serial.print(PANELS[panel].threshold);
    Serial.print(' ');
  }
  Serial.print('\n');
}

void printValues() {
  Serial.print("v ");
  for (uint8_t panel = 0; panel < NUM_STEPS; panel++) {
    Serial.print(PANELS[panel].value);
    Serial.print(' ');
  }
  Serial.print('\n');
}

void writeThreshold(size_t bytes_read) {
  if (bytes_read < 3) return;

  uint8_t index = read_buffer[2] - '0';

  PANELS[index].threshold = atoi(read_buffer + 3);
}

void tickSerial() {
  frames++;

  while (Serial.available() > 0) {
    size_t bytes_read = Serial.readBytesUntil('\n', read_buffer, BUFFER_SIZE - 1);
    read_buffer[bytes_read] = '\0';

    if (bytes_read == 0) return;

    switch (read_buffer[0]) {
    case 'f':
      printFps();
      break;
    case 't':
      printThresholds();
      break;
    case 'v':
      printValues();
      break;
    case 'T':
      writeThreshold(bytes_read);
      break;
    }
  }
}

#endif
