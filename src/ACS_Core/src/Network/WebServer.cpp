#include "Network/WebServer.h"
#define TAG "WebServer"

AsyncWebServer WebServer::server(80);
String WebServer::CommKey = "";

bool WebServer::Initialize()
{
    ESP_LOGI(TAG, "Initializing WebServer");
    WebServerConfig config = GetConfig();
    if(strlen(config.CommKey) == 0)
    {
        ESP_LOGE(TAG, "No WebServer config found");
        return false;
    }

    CommKey = config.CommKey;

    server.on("/", HTTP_GET, HandleRoot);
    server.on("/action", HTTP_POST, HandleAction);

    server.begin();

    return true;
}

// HANDLERS

void WebServer::HandleRoot(AsyncWebServerRequest *request)
{
    request->send(200, "text/plain", "Uptime: " + String(millis()));
}

void WebServer::HandleAction(AsyncWebServerRequest *request)
{
    if(!ValidateRequest(request)) return;

    String action = request->arg("action");

    ActionRequestResult actionResult;

    if(action == "lock") actionResult = ActionHandler::Lock();
    else if(action == "unlock") actionResult = ActionHandler::Unlock();
    else if(action == "engage") actionResult = ActionHandler::Engage();
    else
    {
        request->send(400, "text/plain", "Invalid action");
        return;
    }

    request->send(200, "text/plain", String(actionResult));
}


// VALIDATE
bool WebServer::ValidateRequest(AsyncWebServerRequest *request)
{
    if(!request->hasHeader("CommKey"))
    {
        ESP_LOGI(TAG, "CommKey not found in request");
        request->send(401, "text/plain", "Unauthorized");
        return false;
    }

    String _commKey = request->header("CommKey");

    if(_commKey != CommKey)
    {
        ESP_LOGI(TAG, "Invalid CommKey: %s", _commKey.c_str());
        request->send(401, "text/plain", "Unauthorized");
        return false;
    }

    return true;
}



// CONFIG

WebServerConfig WebServer::GetConfig()
{
    if(!DataSaving::FileExists("/webserverconfig.bin"))
    {
        ESP_LOGW(TAG, "WebServer config file not found");
        return WebServerConfig();
    }

    WebServerConfig config;

    uint16_t readLen = DataSaving::ReadData("/webserverconfig.bin", (uint8_t*)&config, sizeof(WebServerConfig));

    if(readLen != sizeof(WebServerConfig))
    {
        ESP_LOGE(TAG, "Failed to read WebServer config file");
        return WebServerConfig();
    }

    return config;
}

bool WebServer::SetConfig(WebServerConfig config)
{
    ESP_LOGI(TAG, "Setting WebServer config (CommKey: %s)", config.CommKey);
    return DataSaving::WriteData("/webserverconfig.bin", (uint8_t*)&config, sizeof(WebServerConfig), true) == sizeof(WebServerConfig);
}

WebServerConfig::WebServerConfig(const char *_commKey)
{
    memset(CommKey, 0, 33);

    strcpy(CommKey, _commKey);
}

WebServerConfig::WebServerConfig()
{
    memset(CommKey, 0, 33);
}