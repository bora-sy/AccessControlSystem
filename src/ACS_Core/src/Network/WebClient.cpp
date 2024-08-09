#include "Network/WebClient.h"

String WebClient::commKey;
String WebClient::baseURL;

void WebClient::Initialize()
{
    WebConfig config = Config::webConfig;

    commKey = config.CommKey;
    baseURL = RemoteLogging::FormatString("http://%d.%d.%d.%d:80", config.ServerIP[0],config.ServerIP[1] ,config.ServerIP[2], config.ServerIP[3]);

    REMOTELOG_I("WebClient Initialized (CommKey: %s / BaseURL: %s)", commKey, baseURL);
}

WebResponse WebClient::SendRequest(String path, char* method)
{
    HTTPClient client;
    client.setConnectTimeout(REQUEST_TIMEOUT);

    String url = baseURL + path;


    if(!client.begin(url))
    {
        REMOTELOG_W("HTTPClient failed to begin. (URL: %s / Method: %d)", path, method);
        return WebResponse();
    }
    
    int httpCode = client.sendRequest(method);
    if(httpCode == -1)
    {
        REMOTELOG_W("Failed to send HTTP Request. (URL: %s / Method: %d)", path, method);
        return WebResponse();
    }

    String strResponse = client.getString();


    client.end();

    return WebResponse(strResponse, httpCode);


}



WebResponse::WebResponse()
{

}

WebResponse::WebResponse(String _responseData, int _httpCode)
{
    this->ResponseData = _responseData;
    this->HttpCode = _httpCode;
}