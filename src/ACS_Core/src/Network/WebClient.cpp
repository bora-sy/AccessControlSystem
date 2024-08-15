#include "Network/WebClient.h"

String WebClient::commKey;

bool WebClient::Initialize()
{
    Melody pingingMelody;
    Melody pingSucMelody;
    Melody pingFailedMelody;
    Melody failedMelody;

    pingingMelody.AddTone(600, 100);
    pingingMelody.AddRest(50);
    pingingMelody.AddTone(1200, 100);

    pingSucMelody.AddTone(600, 100);
    pingSucMelody.AddRest(50);
    pingSucMelody.AddTone(1200, 100);
    pingSucMelody.AddRest(200);
    pingSucMelody.AddTone(1200, 100);

    failedMelody.AddTone(600, 200);
    failedMelody.AddRest(100);
    failedMelody.AddTone(500, 200);
    failedMelody.AddRest(100);
    failedMelody.AddTone(400, 200);

    pingFailedMelody.AddTone(1000, 100);
    pingFailedMelody.AddRest(50);
    pingFailedMelody.AddTone(500, 100);

    REMOTELOG_I("Initializing WebClient");
    WebConfig config = Config::webConfig;
    commKey = config.CommKey;

    bool pingSuc = false;

    for (int i = 0; i < 20; i++)
    {
        config = Config::webConfig;
        REMOTELOG_I("Pinging server [%d] (IP Addr: %d.%d.%d.%d)", i, config.ServerIP[0], config.ServerIP[1], config.ServerIP[2], config.ServerIP[3]);
        MelodyPlayer::PlayMelody(pingingMelody);

        bool pingRes = PingServer();
        if (pingRes)
        {
            pingSuc = true;
            break;
        }
        else
        {
            REMOTELOG_I("Server Ping Failed");

            MelodyPlayer::PlayMelody(pingFailedMelody);
            delay(3000);
        }
    }

    if (!pingSuc)
    {
        MelodyPlayer::PlayMelody(failedMelody);
        REMOTELOG_W("WebClient Initialization Failed (CommKey: %s / Backend IP Address: %d.%d.%d.%d)", commKey.c_str(), config.ServerIP[0], config.ServerIP[1], config.ServerIP[2], config.ServerIP[3]);
        delay(500);
        return false;
    }

    MelodyPlayer::PlayMelody(pingSucMelody);
    REMOTELOG_I("WebClient Initialized (CommKey: %s / Backend IP Address: %d.%d.%d.%d)", commKey.c_str(), config.ServerIP[0], config.ServerIP[1], config.ServerIP[2], config.ServerIP[3]);
    delay(1000);
    return true;
}

WebResponse WebClient::SendRequest(String path, const char *method)
{
    HTTPClient client;
    client.setConnectTimeout(REQUEST_TIMEOUT);
    client.setUserAgent(USER_AGENT);

    String url = BuildURL(path);

    REMOTELOG_V("Sending HTTP Request...", url);

    if (!client.begin(url))
    {
        REMOTELOG_W("HTTPClient failed to begin. (URL: %s / Method: %s)", path.c_str(), method);
        return WebResponse();
    }

    client.addHeader("CommKey", commKey, true);
    int httpCode = client.sendRequest(method);

    REMOTELOG_V("Request Sent (%s) [Result: %d]", url.c_str(), httpCode);

    if (httpCode == -1)
    {
        REMOTELOG_W("Failed to send HTTP Request. (URL: %s / Method: %s)", path.c_str(), method);
        return WebResponse();
    }

    String strResponse = client.getString();

    client.end();

    return WebResponse(strResponse, httpCode);
}

String WebClient::BuildURL(String path)
{
    WebConfig config = Config::webConfig;
    char buf[256];
    memset(buf, 0, 256);
    String str = RemoteLogging::FormatString("http://%d.%d.%d.%d:5243%s", config.ServerIP[0], config.ServerIP[1], config.ServerIP[2], config.ServerIP[3], path.c_str());
    Serial.println("URL: " + str);
    return str;
}

bool WebClient::PingServer()
{
    WebResponse response = SendRequest("/coredevice/ping", "GET");

    if (response.HttpCode != 200)
    {
        REMOTELOG_W("Failed to ping server. (HTTP Code: %d)", response.HttpCode);
        return false;
    }

    REMOTELOG_D("Server pinged successfully.");
    return true;
}

void WebClient::LogAlarm(bool preAlarm, bool alarmStatus)
{
    return;
    
    bool* arr = new bool[2];
    arr[0] = preAlarm;
    arr[1] = alarmStatus;

    xTaskCreate([](void *arg)
    {
        bool* vals = (bool*)arg;

        String url = RemoteLogging::FormatString("/coredevice/logalarm?prealarm=%d&status=%d", (int)vals[0], (int)vals[1]);
        delete[] vals;
        
        WebResponse response = SendRequest(url, "POST");
        if(response.HttpCode != 200)
        {
            REMOTELOG_W("Failed to log alarm to server. (HTTP Code: %d)", response.HttpCode);
        } else REMOTELOG_I("Logged alarm successfully."); 

        vTaskDelete(NULL);

    }, "LogAlarmT", 2048, arr, 1, NULL);
}

//----------------------------------------

WebResponse::WebResponse()
{
}

WebResponse::WebResponse(String _responseData, int _httpCode)
{
    this->ResponseData = _responseData;
    this->HttpCode = _httpCode;
}