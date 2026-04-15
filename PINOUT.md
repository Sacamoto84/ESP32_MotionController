# ESP32 MotionController — используемые пины (GPIO)

Цель: быстрый список занятых GPIO, как они используются в проекте.

Плата по `platformio.ini`: `esp32-s3-devkitc-1` (ESP32‑S3, Arduino).

## Дисплей ST7789 (TFT_eSPI, SPI)

Определено в `include/User_Setup.h`:

| Сигнал | GPIO | Где задано |
|---|---:|---|
| TFT_MOSI | 11 | `include/User_Setup.h` |
| TFT_SCLK | 12 | `include/User_Setup.h` |
| TFT_CS | 10 | `include/User_Setup.h` |
| TFT_DC | 13 | `include/User_Setup.h` |
| TFT_RST | 14 | `include/User_Setup.h` |
| TFT_BL (подсветка) | 38 | `include/User_Setup.h` |

Подсветка также настраивается через PWM в `src/lcd/lcd.cpp` (используется GPIO 38).

## Драйвер шагового двигателя TMC2160 (TMCStepper)

Определено в `include/tmcGlobal.h`, используется в `src/tmc/tmcInit.cpp` и `src/global.cpp`:

| Сигнал | GPIO | Назначение |
|---|---:|---|
| EN_PIN | 16 | Enable |
| DIR_PIN | 15 | Direction |
| STEP_PIN | 7 | Step |

### Диагностика (DIAG)

| Сигнал | GPIO | Где |
|---|---:|---|
| DIAG0 | 40 | `include/tmcGlobal.h` |
| DIAG1 | 39 | `include/tmcGlobal.h` |

### SPI к TMC2160 (software SPI)

| Сигнал | GPIO | Где |
|---|---:|---|
| CS_PIN | 1 | `include/tmcGlobal.h` |
| SW_MOSI | 2 | `include/tmcGlobal.h` |
| SW_MISO | 42 | `include/tmcGlobal.h` |
| SW_SCK | 41 | `include/tmcGlobal.h` |

## Энкодер (EncButton)

Задано в `src/encoder.cpp`:

| Сигнал | GPIO | Комментарий |
|---|---:|---|
| BTN | 4 | кнопка энкодера |
| ENC0 | 5 | канал A |
| ENC1 | 6 | канал B |

## I2C (внешняя EEPROM JC_EEPROM)

Задано в `src/JC_EEPROM.cpp`:

| Сигнал | GPIO | Комментарий |
|---|---:|---|
| SDA | 8 | `Wire.begin(8, 9)` |
| SCL | 9 | `Wire.begin(8, 9)` |

## UART/Serial (важно)

- `Serial.begin(115200)` используется для лога/отладки, но пины не задаются явно (`src/main.cpp`).
- `Serial1.begin(1000000)` вызывается без явного `rx/tx` (`src/main.cpp`), поэтому фактические GPIO зависят от настроек Arduino core/платы.
- В `src/lcd/lcd.cpp` есть вызовы `timber.i.printf(...)`, но `timber.i.begin(...)` в проекте не найден — фактически UART2 может не использоваться.

## Замеченные потенциальные конфликты/моменты

- В `src/main.cpp` есть `constexpr uint8_t btnStart{6};`, но эта константа нигде не используется и совпадает с `ENC1 = 6` из `src/encoder.cpp`.

