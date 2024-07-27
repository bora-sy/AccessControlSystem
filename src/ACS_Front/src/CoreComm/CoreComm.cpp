#include "CoreComm/CoreComm.h"

namespace CoreComm
{
    HardwareSerial CoreSerial;

    void Initialize()
    {
        CoreSerial.begin(115200, SERIAL_8N1, 16, 17);
    }

    bool Ping(uint16_t timeoutMS)
    {
        uint32_t start = millis();
        while (millis() - start < timeoutMS)
        {
            if (CoreSerial.available())
            {
                String response = CoreSerial.readStringUntil('\n');
                if (response == "PONG")
                    return true;
            }
        }
        return false;
    }
}