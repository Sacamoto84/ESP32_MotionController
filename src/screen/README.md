Пример создания меню

---

## Текст

```c++
    actions.type = itemAction::TEXT;
    actions.text = (char*)"Режим: Постоянный";
    menuContinuous.addMenuAction(actions);
```

---

## SWITCH

```c++
    actions.type = itemAction::SWITCH;
    actions.value = &tmcDriverEnable;
    actions.textOn = (char*)"Мотор: Вкл";
    actions.textOff = (char*)"Мотор: Выкл";
    menuContinuous.addMenuAction(actions);
```
---
## EDITINT
```c++
    actions.type = itemAction::EDITINT;
    actions.text = (char*)"Скорость: ";
    actions.testSuffix = (char*)"";
    actions.value = &tmcStepperMaxSpeed;
    actions.min = 0;
    actions.max = 100000;
    actions.step = 250;
    menuContinuous.addMenuAction(actions);
```
---
## EDITINT_I32
```c++
    actions.type = itemAction::EDITINT_I32;
    actions.text = (char*)"Амплитуда: ";
    actions.testSuffix = (char*)"";
    actions.value = nullptr;
    actions.value_i32 = &tmcStepperSetTarget;
    actions.min = 0;
    actions.max = 1000000;
    actions.step = 100;
    menuContinuous.addMenuAction(actions);
    actions.value_i32 = nullptr;
```
---
## BUTTON
```c++
    actions.type = itemAction::BUTTON;
    actions.text = "Настройка";
    actions.callback = [](int data)
    {
        Serial2.println("Нажата кнопка: Настройка");
        currentScreen = CONFIG;
        update();
    };
    menuContinuous.addMenuAction(actions);
    actions.callback = nullptr;
```



