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
#include <array>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <experimental/optional>
#include <vector>

#include "pad.hpp"

const uint32_t JSON_BUFFER_SIZE = 10240;

// to receive messages from the microcontroller try:
// {"message":"requestValues"}
// {"message":"requestFPS"}
// {"message":"requestConfig"}
// {"message":"requestConfig", "minified": true}

template<typename T>
T getJsonValue(JsonObjectConst o, const std::string& key1, const std::string& key2, T defaultValue) {
    if (o.containsKey(key1)) {
        return o[key1];
    } else if (o.containsKey(key2)) {
        return o[key2];
    } else {
        return defaultValue;
    }
}

DynamicJsonDocument getConfigJson(bool minified, std::experimental::optional<const char*> message) {
    DynamicJsonDocument doc(JSON_BUFFER_SIZE);

    if (message) {
        doc["message"] = message.value();
    }

    JsonObject configJson = doc.createNestedObject("general");

    configJson[minified ? "gap" : "panelThresholdLiftGap"] = PANEL_THRESHOLD_LIFT_GAP;
    configJson[minified ? "invButton" : "invertedDigitalButtonLogic"] = INVERT_BUTTON_LOGIC;
    configJson[minified ? "smoothsamples" : "analogReadSmoothingSamples"] = ANALOGREAD_SMOOTHING_SAMPLES;

    JsonArray panelsJson = doc.createNestedArray("panels");

    for (const Panel &panel: *PANELS) {
        JsonObject panelJson = panelsJson.createNestedObject();

        panelJson[minified ? "b" : "gamepadButton"] = panel.gamepadButton;
        panelJson[minified ? "lo" : "ledOrder"] = panel.ledOrder;
        panelJson[minified ? "led" : "statusLedPin"] = panel.statusLedPin;
        panelJson[minified ? "o" : "stepOrder"] = panel.stepOrder;
        panelJson[minified ? "p" : "pressed"] = panel.pressed;

        JsonArray sensorsJson = panelJson.createNestedArray("sensors");
        for (const Sensor &sensor: panel.sensors) {
            JsonObject sensorJson = sensorsJson.createNestedObject();

            sensorJson[minified ? "t" : "threshold"] = sensor.threshold;
            sensorJson[minified ? "pin" : "teensyPin"] = sensor.teensyPin;
            sensorJson[minified ? "v" : "value"] = sensor.value;
            sensorJson[minified ? "p" : "pressed"] = sensor.pressed;
        }
    }

    JsonArray buttonsJson = doc.createNestedArray("buttons");

    for (const DigitalButton &button: *BUTTONS) {
        JsonObject buttonJson = buttonsJson.createNestedObject();

        buttonJson[minified ? "p" : "pressed"] = button.pressed;
        buttonJson[minified ? "b" : "gamepadButton"] = button.gamepadButton;
        buttonJson[minified ? "pin" : "teensyPin"] = button.teensyPin;
    }

    JsonObject speakerJson = doc.createNestedObject("speaker");

    speakerJson[minified ? "on" : "speakerEnabled"] = SPEAKER;
    speakerJson[minified ? "pin" : "speakerPin"] = SPEAKER_PIN;
    speakerJson[minified ? "buzzer" : "speakerBuzzerMode"] = SPEAKER_BUZZER;
    speakerJson[minified ? "l" : "speakerClickLength"] = SPEAKER_CLICK_LENGTH;
    speakerJson[minified ? "f" : "speakerClickFrequency"] = SPEAKER_FREQUENCY;

    return doc;
}

void setConfigJson(DynamicJsonDocument inputDoc) {
    if (inputDoc.containsKey("general")) {
        JsonObjectConst generalDoc = inputDoc["general"];

        PANEL_THRESHOLD_LIFT_GAP = getJsonValue(generalDoc, "gap", "panelThresholdLiftGap", PANEL_THRESHOLD_LIFT_GAP);

        INVERT_BUTTON_LOGIC = getJsonValue(generalDoc, "invButton", "invertedDigitalButtonLogic", PANEL_THRESHOLD_LIFT_GAP);

        INVERT_BUTTON_LOGIC = getJsonValue(generalDoc, "invButton", "invertedDigitalButtonLogic", PANEL_THRESHOLD_LIFT_GAP);
    }

    if (inputDoc.containsKey("panels")) {
        PANELS = std::make_unique<std::vector<Panel>>();
        JsonArrayConst panelsDoc = inputDoc["panels"];

        for (JsonObjectConst panelDoc : panelsDoc) {
            auto panel = Panel();
            panel.gamepadButton = getJsonValue<uint8_t>(panelDoc, "b", "gamepadButton", 0);
            panel.ledOrder = getJsonValue<uint8_t>(panelDoc, "lo", "ledOrder", 0);
            panel.statusLedPin = getJsonValue<uint8_t>(panelDoc, "led", "statusLedPin", 0);
            panel.stepOrder = getJsonValue<uint8_t>(panelDoc, "o", "stepOrder", 0);

            JsonArrayConst sensorsDoc = panelDoc["sensors"];
            std::vector<Sensor> sensors;
            for (JsonObjectConst sensorDoc : sensorsDoc) {
                Sensor sensor;
                sensor.threshold = getJsonValue<uint8_t>(sensorDoc, "t", "threshold", 0);
                sensor.teensyPin = getJsonValue<uint8_t>(sensorDoc, "pin", "teensyPin", 0);
                sensors.push_back(sensor);
            }
            panel.sensors = sensors;

            PANELS->push_back(panel);
        }
    }

    if (inputDoc.containsKey("buttons")) {
        BUTTONS = std::make_unique<std::vector<DigitalButton>>();
        JsonArrayConst buttonsDoc = inputDoc["buttons"];
        for (JsonObjectConst buttonDoc : buttonsDoc) {
            auto button = DigitalButton();
            button.gamepadButton = getJsonValue<uint8_t>(buttonDoc, "b", "gamepadButton", 0);
            button.teensyPin = getJsonValue<uint8_t>(buttonDoc, "pin", "teensyPin", 0);

            BUTTONS->push_back(button);
        }
    }

    // we may have changed pins:
    setupSteps();
}