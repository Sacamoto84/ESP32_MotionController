#include "global.h"

// dump eeprom contents, 16 bytes at a time.
// always dumps a multiple of 16 bytes.
// duplicate rows are suppressed and indicated with an asterisk.
void eepromDump(uint32_t startAddr, uint32_t nBytes)
{
    // Заголовок с hex + decimal (как в оригинале)
    timber.println("EEPROM DUMP 0x%04x 0x%04x %d %d\n", 
                   startAddr, nBytes, startAddr, nBytes);

    uint32_t nRows = (nBytes + 15) >> 4;
    uint8_t d[16], last[16] = {0};  // Инициализируем нулями
    uint32_t aLast = startAddr;

    for (uint32_t r = 0; r < nRows; r++) {
        uint32_t a = startAddr + 16 * r;
        eep.read(a, d, 16);

        // Проверяем, отличается ли от предыдущей
        bool same = true;
        for (int i = 0; i < 16; ++i) {
            if (last[i] != d[i]) {
                same = false;
                break;  // Оптимизация
            }
        }

        // Выводим только если: первая, последняя или отличается
        if (!same || r == 0 || r == nRows - 1) {
            // Одна строка: адрес + байты
            timber.print("0x%03x", a);  // 0x0000 - 0xFFFF (4 цифры)
            
            // Пробелы или звёздочка
            timber.print("%s ", (a == aLast + 16 || r == 0) ? "  " : "* ");

            // 16 байт в hex: %02x (2 цифры с ведущим нулем)
            for (int16_t c = 0; c < 16; c++) {
                timber.print("%02x", d[c]);
                if (c == 7) 
                    timber.print("  ");  // Двойной пробел после 8-го
                else 
                    timber.print(" ");
            }
            
            timber.print("\n");  // Новая строка
            aLast = a;
        }

        // Сохраняем для следующего сравнения
        for (int i = 0; i < 16; ++i) {
            last[i] = d[i];
        }
    }
}