#include "IO/Keypad.h"

namespace Keypad
{

    const int rows = 4;
    const int cols = 4;

    char keyMap[rows][cols] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    int rowPins[rows] = {33,32,13,12};
    int colPins[cols] = {14,27,26,25};

    void Initialize()
    {
        for (int i = 0; i < rows; i++)
        {
            pinMode(rowPins[i], INPUT_PULLDOWN);
        }

        for (int i = 0; i < cols; i++)
        {
            pinMode(colPins[i], OUTPUT);
            
            digitalWrite(colPins[i], LOW);
        }
    }

    char GetRawKey()
    {
        char key = '\0';

        for (int i = 0; i <cols; i++)
        {
            digitalWrite(colPins[i], HIGH);
            

            for (int j = 0; j < rows; j++)
            {
                int pin = rowPins[j];
                bool res = digitalRead(pin);
                if (res)
                {
                    key = keyMap[j][i];
                    break;
                    
                }
            }

            digitalWrite(colPins[i], LOW);
        }


        return key;
    }


    char lastKey = '\0';
    ulong lastKeyTime = 0;
    char GetKey()
    {
        char key = GetRawKey();

        if(key == '\0')
        {
            if(millis() - lastKeyTime > 10) lastKey = '\0';
            return '\0';
        }

        if(key == lastKey)
        {
            lastKeyTime = millis();
            return '\0';
        }

        lastKey = key;
        lastKeyTime = millis();
        return key;
    }
}