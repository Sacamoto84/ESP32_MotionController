#ifndef _STATE_
#define _STATE_

#include <iostream>
#include <functional>
#include <vector>
#include <string>

#include <JC_EEPROM.h> // https://github.com/JChristensen/JC_EEPROM
#include <Wire.h>      // https://arduino.cc/en/Reference/Wire

extern JC_EEPROM eep;

// Класс для хранения состояния с поддержкой наблюдателей
template <typename T>
class State
{
public:
    // Конструктор для инициализации состояния
    explicit State(T initialValue)
        : value(initialValue), EEPROM_ADDR(-1)
    {
    }

    // Конструктор с EEPROM-адресом
    State(T initialValue, int addr)
        : value(initialValue), EEPROM_ADDR(addr)
    {
    }

    // Получение текущего значения
    T get() const
    {
        return value;
    }

    // Записать в любом случае
    void set(T newValue)
    {
        value = newValue;
        notifyObservers();
    }

    // Добавление наблюдателя
    void addObserver(const std::function<void(T)> &observer)
    {
        observers.push_back(observer);
    }

    // Уведомление всех наблюдателей о новом значении
    void notifyObservers()
    {
        for (const auto &observer : observers)
        {
            observer(value);
        }
    }

    int readFromEEPROM()
    {
        if (addr == -1)
            return;
        set(eep.readInt(EEPROM_ADDR));
    }

    float readFromEEPROM()
    {
        if (addr == -1)
            return;
        set(eep.readFloat(EEPROM_ADDR));
    }
    
    void saveToEEPROM(int addr, int32_t value)
    {
        if (addr == -1)
            return;
        eep.writeInt(addr, value);
    }

    void saveToEEPROM(int addr, float value)
    {
        if (addr == -1)
            return;
        eep.writeFloat(addr, value);
    }

    int EEPROM_ADDR;

private:
    T value;
    std::vector<std::function<void(T)>> observers;
};

#endif