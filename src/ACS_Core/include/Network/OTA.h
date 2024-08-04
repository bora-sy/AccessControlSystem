#pragma once
#include <ElegantOTA.h>
#include "DataSaving.h"
#include <esp_log.h>

struct OTAConfig
{
    char Username[20];
    char Password[20];

    OTAConfig(const char* _username, const char* _password);
    OTAConfig();
};


class OTA
{
    private:
    static AsyncWebServer OTAServer;
    static OTAConfig GetConfig();
    static bool SetConfig(OTAConfig config);

    public:
    static bool Initialize();
    static void Loop();
};
