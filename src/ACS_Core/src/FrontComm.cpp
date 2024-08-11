#include "FrontComm.h"


// Initialization
void FrontComm::Initialize()
{
    FrontSerial.begin(9600, SERIAL_8N1, 16, 17);

    xTaskCreate(t_IncomingHandler, "IncomingHandler", 4096, NULL, 0, NULL);
}

// Actions

bool FrontComm::Ping(uint16_t timeoutMS)
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

void FrontComm::Reset()
{
    SendData(CommandID::RESET, nullptr, 0);
}

void FrontComm::SendFeedback(uint16_t durationMS, const char* feedback)
{
    uint8_t feedbackLen = strlen(feedback);

    uint8_t data[feedbackLen + 2];

    data[0] = durationMS & 0xFF;
    data[1] = durationMS >> 8;

    memcpy(&data[2], feedback, feedbackLen);

    SendData(CommandID::FEEDBACK, data, feedbackLen + 2);
}

void FrontComm::OpenConfigMenu(uint64_t auth)
{
    uint8_t data[8];

    memcpy(data, &auth, 8);

    SendData(CommandID::CONFIGMENU, data, 8);
}

// Handler

void FrontComm::HandleCommand(CommandID commandId, uint8_t *data, uint8_t length)
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

    case CommandID::PASSWORDENTRY:
        if(length != 8)
        {
            Serial.println("Invalid password entry length");
            return;
        }

        uint64_t password;
        memcpy(&password, data, 8);

        Serial.println("Received password entry: " + String(password));
        SendFeedback(2000, "Correct Password");
        ActionHandler::Unlock(ActionSource::Front);
    break;

    case CommandID::CARDENTRY:
    if(length != 10) 
    {
        Serial.println("Invalid card entry length");
        return;
    }

    uint8_t cardUID[10];
    memcpy(cardUID, data, 10);
    Serial.printf("Received card entry: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n", cardUID[0], cardUID[1], cardUID[2], cardUID[3], cardUID[4], cardUID[5], cardUID[6], cardUID[7], cardUID[8], cardUID[9]);
    SendFeedback(2000, "Correct Card");
    ActionHandler::Unlock(ActionSource::Front);
    break;

    }
}

// Communication

HardwareSerial FrontComm::FrontSerial(2);
bool FrontComm::PongReceived = false;

void FrontComm::SendData(CommandID commandId, uint8_t *data, uint8_t length)
{
    FrontSerial.write(length + 1);
    FrontSerial.write(commandId);
    FrontSerial.write(data, length);
}

uint8_t FrontComm::GetIncomingDataLength()
{
    while (true)
    {
        if (FrontSerial.available())
        {
            int rec = FrontSerial.read();

            if(rec == -1) continue;
            return rec;
        }

        delay(5);
    }
}

uint8_t FrontComm::GetIncomingData(uint8_t *data, uint8_t length)
{
    ulong lastByteReceive = millis();

    uint8_t nextByte = 0;
    while (nextByte < length)
    {
        if (FrontSerial.available())
        {
            nextByte += FrontSerial.readBytes(&data[nextByte], length - nextByte);
            lastByteReceive = millis();
        }

        delay(5);

        if(millis() - lastByteReceive > 1000)
        {
            FrontSerial.flush();
            Serial.println("SERIAL COMM TIMEOUT, FLUSHED");
            return 0;
        }
    }

    return nextByte;
}

void FrontComm::t_IncomingHandler(void *args)
{
    while (true)
    {
        uint8_t len = GetIncomingDataLength();
        if(len == 0) continue;

        uint8_t data[len];
        uint8_t dataLen = GetIncomingData(data, len);

        if(dataLen == 0) continue;

        HandleCommand((CommandID)data[0], &data[1], len - 1);
        delay(5);
    }
}