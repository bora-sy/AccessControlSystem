#include "Network/WebClient.h"

String WebClient::commKey;

bool WebClient::Initialize()
{
    REMOTELOG_I("Initializing WebClient");

    WebConfig config = Config::webConfig;

    commKey = config.CommKey;
    
    bool pingRes = PingServer();

    if(!pingRes)
    {
        REMOTELOG_W("WebClient Initialization Failed. Failed to ping server. (CommKey: %s / Backend IP Address: %d.%d.%d.%d)", commKey.c_str(), config.ServerIP[0],config.ServerIP[1] ,config.ServerIP[2], config.ServerIP[3]);
        return false;
    }

    REMOTELOG_I("WebClient Initialized (CommKey: %s / Backend IP Address: %d.%d.%d.%d)", commKey.c_str(), config.ServerIP[0],config.ServerIP[1] ,config.ServerIP[2], config.ServerIP[3]);

    return true;
}

WebResponse WebClient::SendRequest(String path, const char* method)
{
    HTTPClient client;
    client.setConnectTimeout(REQUEST_TIMEOUT);
    client.setUserAgent(USER_AGENT);

    String url = BuildURL(path);

    REMOTELOG_V("Sending HTTP Request...", url);

    if(!client.begin(url))
    {
        REMOTELOG_W("HTTPClient failed to begin. (URL: %s / Method: %s)", path.c_str(), method);
        return WebResponse();
    }

    client.addHeader("CommKey", commKey, true);
    int httpCode = client.sendRequest(method);

    REMOTELOG_V("Request Sent (%s) [Result: %d]", url.c_str(), httpCode);

    if(httpCode == -1)
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
    memset(buf,0, 256);
    String str = RemoteLogging::FormatString("http://%d.%d.%d.%d:5243%s", config.ServerIP[0],config.ServerIP[1] ,config.ServerIP[2], config.ServerIP[3], path.c_str());
    Serial.println( "URL: " + str);
    return str;
}

bool WebClient::PingServer()
{
    WebResponse response = SendRequest("/coredevice/ping", "GET");

    if(response.HttpCode != 200)
    {
        REMOTELOG_W("Failed to ping server. (HTTP Code: %d)", response.HttpCode);
        return false;
    }

    REMOTELOG_D("Server pinged successfully.");
    return true;
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