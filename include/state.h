#ifndef _STATE_
#define _STATE_

#include <iostream>
#include <functional>
#include <vector>
#include <string>

#include <JC_EEPROM.h> // https://github.com/JChristensen/JC_EEPROM
#include <Wire.h>      // https://arduino.cc/en/Reference/Wire

#include "Timber.h"

extern Timber timber;

extern JC_EEPROM eep;

static constexpr uint32_t MAGIC = 0x12345678;
static constexpr int HEADER_ADDR_OFFSET = 0;
static constexpr int VALUE_ADDR_OFFSET = 4;

// Вспомогательный трейт для определения, поддерживается ли тип в EEPROM
template <typename T>
struct EEPROM_Traits
{
    static constexpr bool supported = false;
};

// Специализация для int32_t
template <>
struct EEPROM_Traits<int32_t>
{
    static constexpr bool supported = true;
    static void write(JC_EEPROM &e, int addr, int32_t value) { e.writeInt(addr, value); }
    static int32_t read(JC_EEPROM &e, int addr) { return e.readInt(addr); }
    static constexpr size_t size = 4;
};

// Специализация для float
template <>
struct EEPROM_Traits<float>
{
    static constexpr bool supported = true;
    static void write(JC_EEPROM &e, int addr, float value) { e.writeFloat(addr, value); }
    static float read(JC_EEPROM &e, int addr) { return e.readFloat(addr); }
    static constexpr size_t size = 4;
};

// Класс для хранения состояния с поддержкой наблюдателей
template <typename T>
class State
{
public:
    // Конструктор для инициализации состояния
    explicit State(T initialValue)
        : value(initialValue), base_addr(-1)
    {
    }

    // Конструктор с EEPROM-адресом
    State(T initialValue, int addr)
        : value(initialValue), base_addr(addr)
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

    //--------------------------------

    void init(T defaultValue)
    {
        uint32_t magic = eep.readInt(base_addr);

        if (magic != MAGIC)
        {
            timber.i("init !Magic создаем поле в EEPROM");
            // Первый запуск — записываем магию и значение
            eep.writeInt(base_addr, MAGIC);
            this->save();
            this->set(defaultValue); // Это вызовет запись в VALUE_ADDR
        }
        else
        {
            this->load(); // Загружаем значение
        }
    }

    void save()
    {
        if (base_addr == -1)
            return;
        EEPROM_Traits<T>::write(eep, base_addr+VALUE_ADDR_OFFSET, value);
    }

    void load()
    {
        if (base_addr == -1)
            return;
        set(EEPROM_Traits<T>::read(eep, base_addr+VALUE_ADDR_OFFSET));
    }

private:
    int base_addr;
    T value;
    std::vector<std::function<void(T)>> observers;
};

#endif