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

WebResponse WebClient::SendRequest(String path, HTTPMethod method)
{
    
    HTTPClient client;

    client.begin(baseURL + path);

}