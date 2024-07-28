#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

namespace LCD
{
    extern const int LCD_COLS;
    extern const int LCD_ROWS;

    bool Initialize();

    void ClearRow(uint8_t row);

    void Clear();

    void SetBacklight(bool state);



    void Refresh();

    void ToggleBlink(bool state);

    void SetCursor(uint8_t col, uint8_t row);

    void Print(String str, uint8_t col = 0, uint8_t row = 0, bool clear = true);
    void Print(const char* str, uint8_t col = 0, uint8_t row = 0, bool clear = true);

    void PrintCenterRow(String str, uint8_t row = 0, bool clear = true, uint8_t offset = 0);
    void PrintCenterRow(char* str, uint8_t row = 0, bool clear = true, uint8_t offset = 0);
    void PrintCenterRow(const char* str, uint8_t row = 0, bool clear = true, uint8_t offset = 0);

    void PrintCenter(String string, bool clear = true);
    void PrintCenter(char* str, bool clear = true);
    void PrintCenter(const char* str, bool clear = true);



    void LCDCharTest(uint8_t triggerButtonPin);

}