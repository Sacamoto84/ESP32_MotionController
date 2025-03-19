#include "ble.h"

/**
* Запуск задачи для работы BLE
*/
void Ble::init(void)
{
    xTaskCreatePinnedToCore(
    [](void *parameter) {
        static_cast<Ble *>(parameter)->TaskBleLoop(parameter);
    },   /* Функция для задачи */
        "TaskBleLoop", /* Имя задачи */
        10000,         /* Размер стека */
        NULL,          /* Параметр задачи */
        1,             /* Приоритет */
        &TaskBle,      /* Выполняемая операция */
        0);            /* Номер ядра, на котором она должна выполняться */
}
