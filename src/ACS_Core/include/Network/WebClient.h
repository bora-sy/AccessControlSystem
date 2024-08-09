#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "RemoteLogging.h"

#define REQUEST_TIMEOUT 3000
#define USER_AGENT "ACS_CORE"

struct WebResponse
{
    String ResponseData;
    int HttpCode = -1;

    WebResponse();

    WebResponse(String _responseData, int _httpCode);
};

class WebClient
{
    private:
    static String commKey;

    static WebResponse SendRequest(String path, const char* method);

    static String BuildURL(String path);

    public:
    static bool Initialize();

    static bool PingServer();
};

