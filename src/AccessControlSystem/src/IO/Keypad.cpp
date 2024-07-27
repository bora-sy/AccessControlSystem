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

    int rowPins[rows] = {33,32,35,34};
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

    char GetKey()
    {
        char key = '\0';

        for (int i = 0; i <cols; i++)
        {
            digitalWrite(colPins[i], HIGH);

            for (int j = 0; j < rows; j++)
            {
                if (digitalRead(rowPins[j]) == HIGH)
                {
                    key = keyMap[j][i];
                    Serial.printf("%c (%d, %d)\n",key, i, j);
                    break;
                }
            }

            digitalWrite(colPins[i], LOW);

            if (key != '\0')
            {
                break;
            }
        }

        return key;
    }
}