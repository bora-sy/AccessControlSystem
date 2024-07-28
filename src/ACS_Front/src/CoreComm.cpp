#include "CoreComm.h"

// Variables

char CoreComm::feedback[33];

uint64_t CoreComm::feedbackEndMS = 0;

uint64_t CoreComm::configMenuAuth = 0;

// Initialization
void CoreComm::Initialize()
{
    CoreSerial.begin(115200, SERIAL_8N1, 16, 17);

    xTaskCreate(t_IncomingHandler, "IncomingHandler", 4096, NULL, 0, NULL);
}

// Actions

bool CoreComm::Ping(uint16_t timeoutMS)
{
    SendData(CommandID::PING, nullptr, 0);
    Serial.println("Sent ping");
    uint64_t startMS = millis();
    while (!PongReceived && millis() - startMS < timeoutMS)
    {
        delay(5);
    }

    bool result = PongReceived;
    Serial.println("Ping result: " + String(result));
    PongReceived = false;
    return result;
}

void CoreComm::Reset()
{
    SendData(CommandID::RESET, nullptr, 0);
}

void CoreComm::PasswordEntry(uint64_t password)
{
    uint8_t data[8];
    *((uint64_t *)data) = password;

    SendData(CommandID::PASSWORDENTRY, data, 8);
}

void CoreComm::CardEntry(uint8_t cardUID[10])
{
    SendData(CommandID::CARDENTRY, cardUID, 10);
}

void CoreComm::PlayTone(uint16_t freq, uint16_t duration)
{
    uint8_t data[4];
    memcpy(data, &freq, 2);
    memcpy(data + 2, &duration, 2);

    SendData(CommandID::PLAYTONE, data, 4);
}

void CoreComm::Config_SetIP(uint8_t ip[4])
{
    SendData(CommandID::CONFIG_SETIP, ip, 4);
}

// Handler

void CoreComm::HandleCommand(CommandID commandId, uint8_t *data, uint8_t length)
{
    Serial.println("Handling Command");
    switch (commandId)
    {
    case CommandID::PING:
    {
        Serial.println("Received Ping");
        SendData(CommandID::PONG, data, length);
    }
    break;

    case CommandID::PONG:
    {
        Serial.println("Received Pong");
        PongReceived = true;
    }
    break;

    case CommandID::RESET:
    {
        Serial.println("Received Reset");
        ESP.restart();
    }
    break;

    case CommandID::FEEDBACK:
    {
        Serial.println("Received Feedback");

        uint16_t durationMS = data[0] | (data[1] << 8);
        Serial.println("Duration: " + String(durationMS));
        
        memset(feedback, 0, 33);
        memcpy(feedback, &data[2], length - 2);
        
        feedbackEndMS = millis() + durationMS;
    }
    break;

    case CommandID::CONFIGMENU:
    {
        Serial.println("Received ConfigMenu");
        configMenuAuth = *((uint64_t *)data);
    }
    break;
    }
}

// Communication

HardwareSerial CoreComm::CoreSerial(2);
bool CoreComm::PongReceived = false;

void CoreComm::SendData(CommandID commandId, uint8_t *data, uint8_t length)
{
    CoreSerial.write(length + 1);
    CoreSerial.write(commandId);
    CoreSerial.write(data, length);
}

uint8_t CoreComm::GetIncomingDataLength()
{
    while (true)
    {
        if (CoreSerial.available())
        {
            int rec = CoreSerial.read();

            if(rec == -1) continue;
            return rec;
        }

        delay(5);
    }
}

uint8_t CoreComm::GetIncomingData(uint8_t *data, uint8_t length)
{
    uint8_t nextByte = 0;
    while (nextByte < length)
    {
        if (CoreSerial.available())
        {
            nextByte += CoreSerial.readBytes(&data[nextByte], length - nextByte);
        }
        delay(5);
    }

    return nextByte;
}

void CoreComm::t_IncomingHandler(void *args)
{
    while (true)
    {
        uint8_t len = GetIncomingDataLength();
        uint8_t data[len];
        uint8_t dataLen = GetIncomingData(data, len);

        HandleCommand((CommandID)data[0], &data[1], len - 1);
        delay(5);
    }
}