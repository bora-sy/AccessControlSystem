#include "CoreComm.h"

namespace CoreComm
{
    HardwareSerial CoreSerial(2);

    void t_IncomingHandler(void *args);

    void Initialize()
    {
        CoreSerial.begin(115200, SERIAL_8N1, 16, 17);

        xTaskCreate(t_IncomingHandler, "IncomingHandler", 4096, NULL, 0, NULL);
    }

    void HandleData(uint8_t *data, uint8_t length)
    {
        
    }
    
    uint8_t GetIncomingDataLength()
    {
        while(true)
        {
            if(CoreSerial.available())
            {
                uint8_t len;
                CoreSerial.readBytes(&len, 1);
                return len;
            }

            delay(5);
        }
    }

    uint8_t GetIncomingData(uint8_t *data, uint8_t length)
    {
        uint8_t nextByte = 0;
        while(nextByte < length)
        {
            if(CoreSerial.available())
            {
                nextByte += CoreSerial.readBytes(&data[nextByte], length - nextByte);
            }
            delay(5);
        }

        return nextByte;
    }



    void t_IncomingHandler(void *args)
    {
        while (true)
        {
            uint8_t len = GetIncomingDataLength();
            uint8_t data[len];
            GetIncomingData(data, len);

            HandleData(data, len);
            delay(5);
        }
    }

}