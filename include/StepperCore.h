/*
    Ядро библиотеки для управления шаговыми моторами:
    - 4 фазные и STEP DIR драйверы
    - Поддержка пина EN
    - Виртуальный драйвер
    - Быстрый алгоритм IO для AVR

    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License
*/

/*
    // ======== ИНИЦИАЛИЗАЦИЯ ========
    Stepper<STEPPER2WIRE> stepper(step, dir);                        // драйвер step-dir
    Stepper<STEPPER2WIRE> stepper(step, dir, en);                    // драйвер step-dir + пин enable
    Stepper<STEPPER4WIRE> stepper(pin1, pin2, pin3, pin4);            // драйвер 4 пин
    Stepper<STEPPER4WIRE> stepper(pin1, pin2, pin3, pin4, en);        // драйвер 4 пин + enable
    Stepper<STEPPER4WIRE_HALF> stepper(pin1, pin2, pin3, pin4);        // драйвер 4 пин полушаг
    Stepper<STEPPER4WIRE_HALF> stepper(pin1, pin2, pin3, pin4, en);    // драйвер 4 пин полушаг + enable

    Stepper<STEPPER2WIRE, STEPPER_VIRTUAL> stepper;                    // виртуальный драйвер step-dir
    Stepper<STEPPER4WIRE, STEPPER_VIRTUAL> stepper;                    // виртуальный драйвер 4 пин

    // ============ КЛАСС ============
    void step();                                // сделать шаг
    void invertEn(bool val);                    // инвертировать поведение EN пина
    void reverse(bool val);                     // инвертировать направление мотора
    void disable();                             // отключить питание и EN
    void enable();                              // включить питание и EN
    void power(bool);                           // переключить питание
    void attachStep(void (*handler)(uint8_t));  // подключить обработчик шага
    void attachPower(void (*handler)(bool));    // подключить обработчик питания

    int32_t pos;                                // текущая позиция в шагах
    int8_t dir;                                 // направление (1, -1)
*/

#ifndef _StepperCore_h
#define _StepperCore_h
#include <Arduino.h>

#include "GStypes.h"

#ifndef DRIVER_STEP_TIME
#define DRIVER_STEP_TIME 4
#endif

#define _PINS_AMOUNT ((_TYPE == STEPPER_PINS) ? (_DRV == 0 ? 2 : 4) : (0))

template <GS_driverType _DRV, GS_driverType _TYPE = STEPPER_PINS>
class Stepper {
   public:
    Stepper(uint8_t pin1 = 255, uint8_t pin2 = 255, uint8_t pin3 = 255, uint8_t pin4 = 255, uint8_t pin5 = 255) {
        if (_TYPE == STEPPER_PINS) {
            if (_DRV == STEPPER2WIRE) {
                configurePin(0, pin1);
                configurePin(1, pin2);
                if (pin3 != 255) {
                    _enPin = pin3;
                    pinMode(_enPin, OUTPUT);
                }
            } else {
                configurePin(0, pin1);
                configurePin(1, pin2);
                configurePin(2, pin3);
                configurePin(3, pin4);
                if (pin5 != 255) {
                    _enPin = pin5;
                    pinMode(_enPin, OUTPUT);
                }
            }
        }
    }

    // сделать шаг
    void step() {
        pos += dir;
        //if (_DRV == STEPPER2WIRE) {  // ~4 + DRIVER_STEP_TIME us
            stepDir();
        //} else {  // ~5.5 us
        //    thisStep += (_globDir ? dir : -dir);
        //    step4();
        //}
    }

    // инвертировать поведение EN пина
    void invertEn(bool val) {
        _enDir = val;
    }

    // инвертировать направление мотора
    void reverse(bool val) {
        if (_DRV == STEPPER2WIRE) {
            if (_TYPE == STEPPER_PINS && _globDir != val) setPin(1, (dir > 0) ^ val);
        }
        _globDir = val;
    }

    // отключить питание и EN
    void disable() {
        //if (_TYPE == STEPPER_PINS) {
            if (_enPin != 255) digitalWrite(_enPin, !_enDir);
        //}
//        else {
//            if (*_power) _power(false);
//            if (*_step && (_DRV == STEPPER4WIRE || _DRV == STEPPER4WIRE_HALF)) _step(0);
//        }
    }

    // включить питание и EN
    void enable() {
        //if (_TYPE == STEPPER_PINS) {
            // подадим прошлый сигнал на мотор, чтобы вал зафиксировался
            //if (_DRV == STEPPER4WIRE || _DRV == STEPPER4WIRE_HALF) step4();
            if (_enPin != 255) digitalWrite(_enPin, _enDir);
        //} else {
        //    if (*_power) _power(true);
            //if (*_step && (_DRV == STEPPER4WIRE || _DRV == STEPPER4WIRE_HALF)) step4();
        //}
    }

    // переключить питание
    void power(bool state) {
        if (state) enable();
        else disable();
    }

    // подключить обработчик шага
    void attachStep(void (*handler)(uint8_t)) {
        _step = handler;
    }

    // подключить обработчик питания
    void attachPower(void (*handler)(bool)) {
        _power = handler;
    }

    int32_t pos = 0;
    int8_t dir = 1;

   private:

    // настройка пина
    void configurePin(int num, uint8_t pin) {
        pinMode(pin, OUTPUT);
        _pins[num] = pin;
    }

    // быстрая установка пина
    void setPin(int num, bool state) {
        digitalWrite(_pins[num], state);
    }

    // шажочек степдир
    void stepDir() {
        //if (_TYPE == STEPPER_PINS)
        //{
            if (_pdir != dir) {
                _pdir = dir;
                setPin(1, (dir > 0) ^ _globDir);  // DIR
            }

            digitalWrite(_pins[0], 0);
            //setPin(0, 0);  // step HIGH

            //if (DRIVER_STEP_TIME > 0)
                //delayMicroseconds(DRIVER_STEP_TIME);

            //setPin(0, 1);  // step LOW
            digitalWrite(_pins[0], 1);

        //}
        //else if (*_step) {
        //    _step((dir > 0) ^ _globDir);
        //}
    }

    int8_t _enPin = 255;
    bool _enDir = false;
    bool _globDir = false;
    int8_t _pdir = 0;
    int8_t thisStep = 0;

    void (*_step)(uint8_t a) = nullptr;
    void (*_power)(bool a) = nullptr;

#ifdef __AVR__
    volatile uint8_t *_port_reg[_PINS_AMOUNT];
    volatile uint8_t _bit_mask[_PINS_AMOUNT];
#else
    uint8_t _pins[_PINS_AMOUNT];
#endif
};

#endif
