#pragma once
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "DataSaving.h"
#include "Config.h"
#include "RemoteLogging.h"


class WebServer
{
private:

    static bool ValidateRequest(AsyncWebServerRequest *request);
    
    static AsyncWebServer server;

    static void HandleRoot(AsyncWebServerRequest *request);
    static void HandlePing(AsyncWebServerRequest *request);
    static void HandleAction(AsyncWebServerRequest *request);

    static String CommKey;

public:
    static void Initialize();
};