#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "RemoteLogging.h"

struct WebResponse
{
    String responseData;
    int httpCode;
};

enum HTTPMethod
{
    GET,
    POST
};

class WebClient
{
    private:
    static String commKey;
    static String baseURL;

    static WebResponse SendRequest(String path);

    public:
    static void Initialize();
};

