#ifndef UTILS_LOGUART_H_
#define UTILS_LOGUART_H_

#include <Arduino.h>

#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#define COLOR_WARNING "\33[38;05;11m"
#define COLOR_ERROR   "\33[38;05;9m"
#define COLOR_RESET   "\33[0m"

class Timber {
public:

    static void clear() { print("\33[1m\n"); }   //Очистка терминала
    //void color(int color = 15);                //Задать цвет текста
    //void bgcolor(int color = 0);               //Задать цвет фона

    static void reset() { print("\33[0m"); };

    static void resetln() { print("\33[0m\n"); };

    static void print(char const *format) { Serial1.write((uint8_t *) format, strlen(format)); }
    static void print(const String &format) { Serial1.write(format.c_str()); }
    static void print(const StringSumHelper &format) { Serial1.write(format.c_str()); }

    template<typename ... Args>
    void print(char const *const format, Args const &... args) noexcept {
        sprintf(str, format, args ...);
        Serial.write((uint8_t *) str, strlen(str));
    }

    template<typename ... Args>
    void println(char const *const format, Args const &... args) noexcept {
        sprintf(str, format, args ...);
        strcat(str, "\n");
        Serial.write((uint8_t *) str, strlen(str));
    }

    void println(char const *format) {
        Serial.write((uint8_t *) format, strlen(format));
        Serial.write('\n');
    }

    void println(const String &format) {
        Serial.write(format.c_str());
        Serial.write('\n');
    }

    void println(const StringSumHelper &format) {
        Serial.write(format.c_str());
        Serial.write('\n');
    }

    void println(String *format) {
        Serial.write(format->c_str());
        Serial.write('\n');
    }

    template<typename ... Args>
    void w(char const *const format, Args const &... args) noexcept {
        sprintf(str, format, args ...);
        colorStringln(11, str);
    }

    template<typename ... Args>
    void e(char const *const format, Args const &... args) noexcept {
        sprintf(str, format, args ...);
        colorStringln(9, str);
    }

    template<typename ... Args>
    void i(char const *const format, Args const &... args) noexcept {
        sprintf(str, format, args ...);
        colorStringln(45, str);
    }

    template<typename ... Args>
    void s(char const *const format, Args const &... args) noexcept {
        sprintf(str, format, args ...);
        colorStringln(10, str);
    }

    char str[512];

    void w(char const *const format) { colorStringln(11, format); }

    void e(char const *const format) { colorStringln(9, format); }

    void i(char const *const format) { colorStringln(45, format); }
    void i(const String &format)     { Serial1.print(format); }

    void s(char const *const format) { colorStringln(10, format); }

    void setBold() {
        char s[16];
        sprintf(s, "\33[01m");
        print(s);
    }

    void setItalic() {
        char s[16];
        sprintf(s, "\33[03m");
        print(s);
    }

    void setUnderline() {
        char s[16];
        sprintf(s, "\33[04m");
        print(s);
    }

    void setRevers() {
        char s[16];
        sprintf(s, "\33[07m");
        print(s);
    }

    void setFlash() {
        char s[16];
        sprintf(s, "\33[08m");
        print(s);
    }

    void color(int color) {
        char s[16];
        sprintf(s, "\33[38;05;%dm", color);
        print(s);
    }

    void bgcolor(int color) {
        char s[16];
        sprintf(s, "\33[48;05;%dm", color);
        print(s);
    }

    void colorString(int c, char const *const format) {
        color(c);
        print(format);
        print("\33[0m");
    }

    void colorString(int color, int bgcolor, char const *const format) {
        char s[32];
        sprintf(s, "\33[38;05;%d;48;05;%dm", color, bgcolor);
        print(s);
        print(format);
        print("\33[0m");
    }

    void colorStringln(int c, char const *const format) {
        color(c);
        print(format);
        print("\33[0m\n");
    }

    void colorStringln(int color, int bgcolor, char const *const format) {
        char s[32];
        sprintf(s, "\33[38;05;%d;48;05;%dm", color, bgcolor);
        print(s);
        print(format);
        print("\33[0m\n");
    }

};

extern Timber timber;

#endif /* UTILS_LOGUART_H_ */
