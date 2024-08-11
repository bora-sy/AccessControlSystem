#pragma once
#include <ElegantOTA.h>
#include "DataSaving.h"
#include "Config.h"
#include "RemoteLogging.h"

class OTA
{
    private:
    static AsyncWebServer OTAServer;
    static void onOTAEnd(bool success);
    static void onOTAProgress(size_t current, size_t final);
    static void onOTAStart();

    public:
    static void Initialize();
    static void Loop();
};
