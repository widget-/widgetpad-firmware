/*
 * Copyright (c) 2021, Widget <https://github.com/widget->
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2. See the
 * included LICENSE file.
*/

#ifndef WP_EEPROM
#define WP_EEPROM

#include <stdint.h>
#include "Arduino.h"
#include "EEPROM.h"

#include "config.hpp"

#define CONFIG_VERSION 1

/*
    The EEPROM data structure is as follows (numbers are bytes):

    (A version number of 0 signified empty EEPROM)

    Version 1:
    [0] : EEPROM data structure version number
    [1] : Number of panels stored
    [2-sizeof(Panel)*NUMBER_OF_PANELS] : Entire panel objects

    This allows for saving everything configurable.

    On startup, the firmware will check for both matching storage version and
    number of panels, and will reset to default if they mismatch.

    There's no bounds checking here but you may run into issues with more than
    a few hundred panels. That said you can only store 256 panels anyway.
*/
struct EepromStructure {
    uint8_t configVersion = CONFIG_VERSION;
    uint8_t numberOfPanels = PANELS->size();
//    Panel panels[NUMBER_OF_PANELS];
};

bool loadEeprom() {
    // EepromStructure eeprom;
    // EEPROM.get(0, eeprom);
    // if (eeprom.configVersion == CONFIG_VERSION &&
    //     eeprom.numberOfPanels == NUMBER_OF_PANELS){
    //     for (uint8_t i = 0; i < NUMBER_OF_PANELS; i++) {
    //         PANELS[i] = eeprom.panels[i];
    //     }
    //     return true;
    // }
    return false;
}

void storeEeprom() {
//    EepromStructure eeprom = EepromStructure();
//    eeprom.configVersion = CONFIG_VERSION;
//    eeprom.numberOfPanels = NUMBER_OF_PANELS;
//    for (uint8_t i = 0; i < NUMBER_OF_PANELS; i++) {
//        eeprom.panels[i] = PANELS[i];
//    }
//    EEPROM.put(0, eeprom);
}

void clearEeprom() {
    for (uint8_t i = 0; i < sizeof(EepromStructure); i++) {
        EEPROM.update(i, 0);
    }
}

void setupEeprom() {
    loadEeprom();
}

#endif
