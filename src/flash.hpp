# pragma once

#include "Arduino.h"
#include <LittleFS.h>
#include "ArduinoJson.h"

#include "config.hpp"
#include "json.hpp"

bool flashStatus;
LittleFS_Program FLASH_MEMORY;

void setupFlash() {
    flashStatus = FLASH_MEMORY.begin(FLASH_FILE_SIZE);
}

// Use with {"message":"saveConfigRequest"}
std::experimental::optional<const char*> writeConfigToFlash() {
    if (!flashStatus) {
        return "Flash is not initialized";
    }

    DynamicJsonDocument doc = getConfigJson(true, NULL);

    File file = FLASH_MEMORY.open("config.json", FILE_WRITE);
    if (!file) {
        return "Unable to open file for writing";
    }

    serializeJson(doc, file);

    file.close();
    return {};
}

// Use with {"message":"loadConfigRequest"}
std::experimental::optional<const char*> loadConfigFromFlash() {
    if (!flashStatus) {
        return "Flash is not initialized";
    }

    File file = FLASH_MEMORY.open("config.json", FILE_READ);
    if (!file) {
        return "Unable to open file for reading";
    }

    DynamicJsonDocument doc(JSON_BUFFER_SIZE);
    DeserializationError err = deserializeJson(doc, file);
    if (err != DeserializationError::Ok) {
        char err_str[255];
        sprintf(err_str, "Error deserializing JSON: %s", err.c_str());
        return err_str;
    }

    setConfigJson(doc);
    file.close();
    return {};
}