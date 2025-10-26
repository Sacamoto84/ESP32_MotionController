#ifndef _STATE_
#define _STATE_

#include <iostream>
#include <functional>
#include <vector>
#include <string>

// Класс для хранения состояния с поддержкой наблюдателей
template <typename T>
class State {
public:
    // Конструктор для инициализации состояния
    explicit State(T initialValue) : value(initialValue) {}

    // Получение текущего значения
    T get() const {
        return value;
    }

    // Установка нового значения с уведомлением наблюдателей
    // void set(T newValue) {
    //     if (value != newValue) {
    //         value = newValue;
    //         notifyObservers();
    //     }
    // }

    //Записать в любом случае
    void set(T newValue)
    {
        value = newValue;
        notifyObservers();
    }

    // Добавление наблюдателя
    void addObserver(const std::function<void(T)>& observer) {
        observers.push_back(observer);
    }

    // Уведомление всех наблюдателей о новом значении
    void notifyObservers() {
        for (const auto& observer : observers) {
            observer(value);
        }
    }

private:
    T value;
    std::vector<std::function<void(T)>> observers;


};

#endif