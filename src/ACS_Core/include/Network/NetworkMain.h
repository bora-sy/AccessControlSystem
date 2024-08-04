#pragma once
#include <WiFi.h>
#include "DataSaving.h"
#include "esp_log.h"

#define WIFI_CONNECTION_TIMEOUT 15000

struct WiFiConfig
{
    char SSID[50];
    char Password[50];

    WiFiConfig(const char* _ssid, const char* _password);
    WiFiConfig();
};

class NetworkMain
{
    private:
    static WiFiConfig GetConfig();
    static bool SetConfig(WiFiConfig config);
    
    public:
    static bool Initialize();
};