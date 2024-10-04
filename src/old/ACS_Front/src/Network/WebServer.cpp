#include "Network/WebServer.h"
#define TAG "WebServer"

AsyncWebServer WebServer::server(80);
String WebServer::CommKey = "";

void WebServer::Initialize()
{
    WebConfig config = Config::webConfig;

    CommKey = config.CommKey;

    server.on("/", HTTP_GET, HandleRoot);
    server.on("/ping", HTTP_GET, HandlePing);
    server.on("/action", HTTP_POST, HandleAction);

    server.begin();

    REMOTELOG_I("WebServer Initialized (CommKey: %s)", CommKey.c_str());
}

// HANDLERS

void WebServer::HandleRoot(AsyncWebServerRequest *request)
{
    REMOTELOG_D("Received Root HTTP Request");
    request->send(200, "text/plain", "Uptime: " + String(millis()));
}

void WebServer::HandlePing(AsyncWebServerRequest *request)
{
    if(!ValidateRequest(request)) return;
    
    REMOTELOG_D("Received Ping HTTP Request");

    uint32_t newIP_32 = request->client()->getRemoteAddress();
    uint8_t* newIP = (uint8_t*)&newIP_32;
    
    uint8_t oldIP[4];
    memcpy(oldIP, Config::webConfig.ServerIP, 4);
    
    
    
    request->send(200, "text/plain", String(millis()));

    if(request->getHeader("User-Agent")->value() != "ACS_Backend") return;
    
    REMOTELOG_I("Backend IP Address Change Check (%d.%d.%d.%d ==> %d.%d.%d.%d)", oldIP[0],oldIP[1],oldIP[2],oldIP[3], newIP[0],newIP[1],newIP[2],newIP[3]);
    
    bool IPChanged = memcmp(newIP, oldIP, 4) != 0;

    if(IPChanged)
    {
    memcpy(Config::webConfig.ServerIP, newIP, 4);

    bool updSuc = Config::UpdateWebConfig();
    if(!updSuc) REMOTELOG_E("Failed to change Backend IP Address");
    else REMOTELOG_I("Backend IP Address Changed (%d.%d.%d.%d ==> %d.%d.%d.%d)", oldIP[0],oldIP[1],oldIP[2],oldIP[3], newIP[0],newIP[1],newIP[2],newIP[3]);
    }
}

// VALIDATE
bool WebServer::ValidateRequest(AsyncWebServerRequest *request)
{
    if(!request->hasHeader("CommKey"))
    {
        REMOTELOG_W("CommKey not found in request");
        request->send(401, "text/plain", "Unauthorized");
        return false;
    }

    String _commKey = request->header("CommKey");

    if(_commKey != CommKey)
    {
        REMOTELOG_W("Invalid CommKey: %s", _commKey.c_str());
        request->send(401, "text/plain", "Unauthorized");
        return false;
    }

    return true;
}
