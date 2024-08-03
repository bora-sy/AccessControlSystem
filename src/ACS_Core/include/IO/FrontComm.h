#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>

enum CommandID : uint8_t
{
    PING = 0,      // Front <--> Core
    PONG,          // Front <--> Core
    RESET,         // Front <--> Core

    FEEDBACK,      // Core   --> Front
    CONFIGMENU,    // Core   --> Front

    PASSWORDENTRY, // Front  --> Core
    CARDENTRY,     // Front  --> Core
    PLAYTONE,      // Front  --> Core

    CONFIG_SETIP,  // Front  --> Core

    

};


class FrontComm
{
private:

    static HardwareSerial CoreSerial;
    static bool PongReceived;

    static void SendData(CommandID commandId, uint8_t *data, uint8_t length);
    static uint8_t GetIncomingDataLength();
    static uint8_t GetIncomingData(uint8_t *data, uint8_t length);
    static void t_IncomingHandler(void *args);
    static void HandleCommand(CommandID commandId, uint8_t *data, uint8_t length);

public:
    static char feedback[33];
    static uint64_t feedbackEndMS;

    static uint64_t configMenuAuth;

    static void Initialize();

    static bool Ping(uint16_t timeoutMS);
    static void Reset();
    static void PasswordEntry(uint64_t password);
    static void CardEntry(uint8_t cardUID[10]);
    static void PlayTone(uint16_t freq, uint16_t duration);
    static void Config_SetIP(uint8_t ip[4]);
};