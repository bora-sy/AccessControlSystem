#pragma once
#include <WiFi.h>
#include "DataSaving.h"
#include "esp_log.h"
#include "Config.h"

#define WIFI_CONNECTION_TIMEOUT 15000

class NetworkMain
{
    private:

    static Melody initMelody;
    static Melody connectedMelody;
    static Melody failedMelody;
    static Melody connectionLostMelody;
    
    public:
    static bool Initialize();
    static void CheckWiFiConnection();
};