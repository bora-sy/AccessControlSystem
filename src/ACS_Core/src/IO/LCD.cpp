#include "IO/LCD.h"

namespace LCD
{
    const int LCD_COLS = 16;
    const int LCD_ROWS = 2;

    LiquidCrystal_I2C display(0x27, LCD_COLS, LCD_ROWS);


    bool Initialize()
    {
        bool suc = display.init();


        display.backlight();
        display.clear();

        return suc;
    }

    void ClearRow(uint8_t row)
    {
        display.setCursor(0, row);
        display.print(F("                "));
        delay(50);
    }

    void Clear()
    {
        display.clear();
        delay(50);
    }
    
    void SetBacklight(bool state)
    {
        if(state) display.backlight();
        else display.noBacklight();
    }

    void Refresh()
    {
        display.Refresh();
    }

    void ToggleBlink(bool state)
    {
        if(state) display.blink();
        else display.noBlink();
    }

    void SetCursor(uint8_t col, uint8_t row)
    {
        display.setCursor(col, row);
    }

    void Print(const char* str, uint8_t col, uint8_t row, bool clear)
    {
        if (clear) ClearRow(row);

        display.setCursor(col, row);
        display.print(str);
    }

    void Print(String str, uint8_t col, uint8_t row, bool clear)
    {
        Print(str.c_str(), col, row, clear);
    }



    void PrintCenterRow(String str, uint8_t row, bool clear, uint8_t offset)
    {
        offset = offset < 0 ? 0 : offset;

        String string(str);
        string.trim();
        if(string.length() > LCD_COLS - offset)
        {
            Print(string, offset, row, clear);
        }

        uint8_t pos = (LCD_COLS - offset - string.length()) / 2;
        if(pos < offset) pos = offset;

        if(string.length() > LCD_COLS - offset) string = string.substring(0, LCD_COLS + 1 - offset);

        Print(string, pos, row, clear);
    }

    void PrintCenterRow(char *str, uint8_t row, bool clear, uint8_t offset)
    {
        String string(str);

        PrintCenterRow(string, row, clear);
    }

    void PrintCenterRow(const char *str, uint8_t row, bool clear, uint8_t offset)
    {
        PrintCenterRow((char *)str, row, clear);
    }


    void PrintCenter(String string, bool clear)
    {
        if (string.length() <= LCD_COLS)
        {
            Clear();
            PrintCenterRow(string, 0, false);
            return;
        }

        for (int i = string.length() - 1; i >= 0; i--)
        {
            if (string.charAt(i) == ' ')
            {
                String first = string.substring(0, i);
                String second = string.substring(i + 1);

                first.trim();
                second.trim();

                if (first.length() <= LCD_COLS && second.length() <= LCD_COLS)
                {
                    if(clear) Clear();
                    PrintCenterRow(first, 0, false);
                    PrintCenterRow(second, 1, false);
                    return;
                }
            }
        }

        PrintCenterRow(string, 0, clear);
    }

    void PrintCenter(char *str, bool clear)
    {
        String string(str);

        PrintCenter(string, clear);
    }

    void PrintCenter(const char *str, bool clear)
    {
        PrintCenter((char *)str, clear);
    }




    void LCDCharTest(uint8_t triggerButtonPin)
    {
        pinMode(triggerButtonPin, INPUT_PULLUP);

        int c;

        while (true)
        {
            char str[17];
            memset(str, 0, 17);
            String ser = "";

            for(int i; i < 16; i++)
            {
                uint8_t num = (c * 16) + i + 1;
                str[i] = num;
                ser += String(num) + " / ";
            }

            PrintCenterRow(str, 0, true);
            Serial.println(ser);


            while (digitalRead(triggerButtonPin)) delay(10);
            delay(200);

            c++;

        }

    }

}