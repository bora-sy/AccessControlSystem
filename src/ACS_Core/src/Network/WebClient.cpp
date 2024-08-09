#include "Network/WebClient.h"

String WebClient::commKey;
String WebClient::baseURL;

bool WebClient::Initialize()
{
    REMOTELOG_I("Initializing WebClient");

    WebConfig config = Config::webConfig;

    commKey = config.CommKey;
    baseURL = RemoteLogging::FormatString("http://%d.%d.%d.%d:80", config.ServerIP[0],config.ServerIP[1] ,config.ServerIP[2], config.ServerIP[3]);

    bool pingRes = PingServer();

    if(!pingRes)
    {
        REMOTELOG_W("WebClient Initialization Failed. Failed to ping server. (CommKey: %s / BaseURL: %s)", commKey, baseURL);
        return false;
    }

    REMOTELOG_I("WebClient Initialized (CommKey: %s / BaseURL: %s)", commKey, baseURL);
}

WebResponse WebClient::SendRequest(String path, char* method)
{
    HTTPClient client;
    client.setConnectTimeout(REQUEST_TIMEOUT);
    client.setUserAgent(USER_AGENT);

    String url = baseURL + path;

    REMOTELOG_V("Sending HTTP Request...", url);

    if(!client.begin(url))
    {
        REMOTELOG_W("HTTPClient failed to begin. (URL: %s / Method: %d)", path, method);
        return WebResponse();
    }
    
    int httpCode = client.sendRequest(method);

    REMOTELOG_V("Request Sent (%s) [Result: %d]", url, httpCode);

    if(httpCode == -1)
    {
        REMOTELOG_W("Failed to send HTTP Request. (URL: %s / Method: %d)", path, method);
        return WebResponse();
    }

    String strResponse = client.getString();

    client.end();

    return WebResponse(strResponse, httpCode);


}

bool WebClient::PingServer()
{
    WebResponse response = SendRequest("/ping", "GET");

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