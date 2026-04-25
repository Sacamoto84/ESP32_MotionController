#ifndef _STATE_
#define _STATE_

#include <iostream>
#include <functional>
#include <vector>
#include <string>

#include <freertos/portmacro.h>

#include "Timber.h"

extern Timber timber;

static constexpr uint32_t MAGIC = 0x12345678;
static constexpr int HEADER_ADDR_OFFSET = 0;
static constexpr int VALUE_ADDR_OFFSET = 4;

// Класс для хранения состояния с поддержкой наблюдателей
template <typename T>
class State
{
public:
    // Конструктор для инициализации состояния
    explicit State(T initialValue)
        : value(initialValue), base_addr(-1), mux(portMUX_INITIALIZER_UNLOCKED)
    {
    }

    // Конструктор с EEPROM-адресом
    State(T initialValue, int addr)
        : value(initialValue), base_addr(addr), mux(portMUX_INITIALIZER_UNLOCKED)
    {
    }

    T get() const
    {
        portENTER_CRITICAL(&mux);
        T v = value;
        portEXIT_CRITICAL(&mux);
        return v;
    }

    void set(T newValue)
    {
        portENTER_CRITICAL(&mux);
        value = newValue;
        auto obs = observers;
        portEXIT_CRITICAL(&mux);
        for (const auto &fn : obs)
        {
            fn(newValue);
        }
    }

    void addObserver(const std::function<void(T)> &observer)
    {
        portENTER_CRITICAL(&mux);
        observers.push_back(observer);
        portEXIT_CRITICAL(&mux);
    }

    void notifyObservers()
    {
        portENTER_CRITICAL(&mux);
        T v = value;
        auto obs = observers;
        portEXIT_CRITICAL(&mux);
        for (const auto &fn : obs)
        {
            fn(v);
        }
    }

    //--------------------------------

    void init(T defaultValue)
    {
        set(defaultValue);
    }

    // Сохранить текущий value в EEPROM
    void save()
    {
        if (base_addr == -1)
            return;
        //EEPROM_Traits<T>::write(eep, base_addr + VALUE_ADDR_OFFSET, value);
    }

    // Прочитать значение из  EEPROM и поместить в value
    void load()
    {
        if (base_addr == -1)
            return;
        //value = EEPROM_Traits<T>::read(eep, base_addr + VALUE_ADDR_OFFSET);
    }

private:
    int base_addr;
    T value;
    std::vector<std::function<void(T)>> observers;
    mutable portMUX_TYPE mux;
};

#endif
