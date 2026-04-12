// addresses.h
#ifndef _ADDRESSES_
#define _ADDRESSES_

#include "Arduino.h"

constexpr uint16_t EEPROM_ADDR(uint16_t base, uint8_t index) {
    return base + index * 8;
}

constexpr uint16_t EEPROM_TMC_BASE = 0x00;

constexpr uint16_t EEPROM_TMC_DRIVER_ENABLE        = EEPROM_ADDR(EEPROM_TMC_BASE, 0);  // 0x00
constexpr uint16_t EEPROM_TMC_DRIVER_CHOP          = EEPROM_ADDR(EEPROM_TMC_BASE, 1);  // 0x08
constexpr uint16_t EEPROM_TMC_DRIVER_CURRENT       = EEPROM_ADDR(EEPROM_TMC_BASE, 2);  // 0x10
constexpr uint16_t EEPROM_TMC_DRIVER_MICROSTEP     = EEPROM_ADDR(EEPROM_TMC_BASE, 3);  // 0x18
constexpr uint16_t EEPROM_TMC_DRIVER_INTERPOLATION = EEPROM_ADDR(EEPROM_TMC_BASE, 4);  // 0x20
constexpr uint16_t EEPROM_TMC_STEPPER_MAXSPEED     = EEPROM_ADDR(EEPROM_TMC_BASE, 5);  // 0x28
constexpr uint16_t EEPROM_TMC_STEPPER_SETTARGET    = EEPROM_ADDR(EEPROM_TMC_BASE, 6);  // 0x30
constexpr uint16_t EEPROM_VIBRO_ANGLE              = EEPROM_ADDR(EEPROM_TMC_BASE, 7);  // 0x38
constexpr uint16_t EEPROM_VIBRO_FREC               = EEPROM_ADDR(EEPROM_TMC_BASE, 8);  // 0x40

#endif