#pragma once
#include <ElegantOTA.h>
#include "DataSaving.h"
#include <esp_log.h>
#include "Config.h"

class OTA
{
    private:
    static AsyncWebServer OTAServer;

    public:
    static bool Initialize();
    static void Loop();
};
