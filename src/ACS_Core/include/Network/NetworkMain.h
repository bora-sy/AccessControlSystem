#pragma once
#include <WiFi.h>
#include "DataSaving.h"
#include "esp_log.h"
#include "Config.h"

#define WIFI_CONNECTION_TIMEOUT 15000

class NetworkMain
{
    private:
    
    public:
    static bool Initialize();
};