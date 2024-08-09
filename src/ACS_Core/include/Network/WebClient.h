#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "RemoteLogging.h"

#define REQUEST_TIMEOUT 3000

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
    static String baseURL;

    static WebResponse SendRequest(String path, char* method);

    public:
    static void Initialize();
};

