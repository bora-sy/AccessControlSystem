#pragma once
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "ActionHandler.h"
#include "DataSaving.h"

struct WebServerConfig
{
    char CommKey[33];

    WebServerConfig(const char* _commKey);
    WebServerConfig();
};


class WebServer
{
private:

    static bool ValidateRequest(AsyncWebServerRequest *request);
    
    static AsyncWebServer server;

    static void HandleRoot(AsyncWebServerRequest *request);
    static void HandleAction(AsyncWebServerRequest *request);
    
    static WebServerConfig GetConfig();
    static bool SetConfig(WebServerConfig config);

    static String CommKey;

public:
    static bool Initialize();
};