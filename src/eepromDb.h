
#ifndef _EEPROM_DB_
#define _EEPROM_DB_



template <int SIZE>
class EEPROM_DB
{

private:
    uint8_t buffer[SIZE]; // буфер из uint8_t

    uint32_t _tmr = 0, _tout = 10000;
    bool _update = 0;

public:
    EEPROM_DB() {}

    void write(int address, uint8_t data)
    {
        if (address < SIZE)
        {
            buffer[address] = data;
        }
    }

    uint8_t read(int address)
    {
        if (address < SIZE)
        {
            return buffer[address];
        }
        return 0;
    }

    // обновить данные в файле, если было изменение БД. Вернёт true при успешной записи
    bool update()
    {
        _tmr = 0;
        if (!_update)
            return false;
        _update = false;
        timber.i("db update");
        auto file = LittleFS.open(_path, "w");
        return file ? writeTo(file) : false;
    }

    // тикер, вызывать в loop. Сам обновит данные при изменении и выходе таймаута, вернёт true
    bool tick()
    {
        if (_update && !_tmr)
        {
            _tmr = millis();
        }
        if (_tmr && millis() - _tmr >= _tout)
        {
            update();
            return true;
        }
        return false;
    }

    int size() const { return SIZE; }
};

#endif // _EEPROM_DB_
