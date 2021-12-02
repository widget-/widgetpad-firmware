/*
 * Copyright (c) 2021, Widget <https://github.com/widget->
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2. See the
 * included LICENSE file.
 */

#pragma once

#include "Arduino.h"
#include "ArduinoJson.h"
#include "config.hpp"
#include "eeprom_save.hpp"
#include "json.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const char *MESSAGE_VALUES_REQUEST = "requestValues";
const char *MESSAGE_VALUES_RESPONSE = "responseValues";
const char *MESSAGE_FPS_REQUEST = "requestFPS";
const char *MESSAGE_FPS_RESPONSE = "responseFPS";
const char *MESSAGE_CONFIG_REQUEST = "requestConfig";
const char *MESSAGE_CONFIG_RESPONSE = "responseConfig";
const char *MESSAGE_CONFIG_UPDATE = "updateConfig";
const char *MESSAGE_ERROR = "deserializationError";

uint32_t frames = 0;

void setupSerial() {
    Serial.setTimeout(1000);
    Serial.begin(SERIAL_BAUD_RATE);
}

void printFps() {
    std::ostringstream output;
    output << "fps " << (float) frames * 1000.0 / (float) millis();
    Serial.println(output.str().c_str());
}

void printValues() {
    DynamicJsonDocument doc(JSON_BUFFER_SIZE);

    doc["message"] = MESSAGE_VALUES_RESPONSE;
    JsonArray panelsJson = doc.createNestedArray("panels");
    for (const Panel &panel: *PANELS) {
        JsonArray panelJson = panelsJson.createNestedArray();
        for (const Sensor &sensor: panel.sensors) {
            JsonObject sensorJson = panelJson.createNestedObject();
            sensorJson["value"] = sensor.value;
            sensorJson["pressed"] = sensor.pressed;
        }
    }

    JsonArray buttonsJson = doc.createNestedArray("buttons");
    for (const DigitalButton &button: *BUTTONS) {
        JsonObject buttonJson = buttonsJson.createNestedObject();
        buttonJson["pressed"] = button.pressed;
    }

    serializeJsonPretty(doc, Serial);
    Serial.print('\n');
}

void printConfig(bool minified) {
    DynamicJsonDocument doc = getConfigJson(minified, MESSAGE_CONFIG_RESPONSE);
    if (minified) {
        serializeJson(doc, Serial);
    } else {
        serializeJsonPretty(doc, Serial);
    }
    Serial.println("");
}

void printError(DeserializationError err) {
    DynamicJsonDocument doc(JSON_BUFFER_SIZE);

    doc["message"] = MESSAGE_ERROR;
    doc["err"] = err.c_str();

    serializeJsonPretty(doc, Serial);
    Serial.print('\n');
}

void saveToEeprom() {
    storeEeprom();
    Serial.print("s ok\n");
}

void loadFromEeprom() {
    if (loadEeprom()) { Serial.print("l ok\n"); }
    else {
        Serial.print("l failed\n");
    }
}

void resetEeprom() {
    clearEeprom();
    Serial.print("c ok\n");
}

void tickSerial() {
    frames++;

    if (Serial.available() < 1) return;

    DynamicJsonDocument inputDoc(JSON_BUFFER_SIZE);
    DeserializationError err = deserializeJson(inputDoc, Serial);

    if (err != DeserializationError::Ok) {
        printError(err);

        // flush remaining buffer
        while (Serial.available() > 0) {
            Serial.read();
        }

        return;
    }

    auto messageType = inputDoc["message"].as<std::string>();
    bool minified = inputDoc.containsKey("minified") && inputDoc["minified"];

    if (messageType == MESSAGE_VALUES_REQUEST) {
        printValues();
    } else if (messageType == MESSAGE_FPS_REQUEST) {
        printFps();
    } else if (messageType == MESSAGE_CONFIG_REQUEST) {
        printConfig(minified);
    } else if (messageType == MESSAGE_CONFIG_UPDATE) {
        setConfigJson(inputDoc);
    }
}
