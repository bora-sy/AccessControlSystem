#include "Network/WebServer.h"
#define TAG "WebServer"

AsyncWebServer WebServer::server(80);
String WebServer::CommKey = "";

void WebServer::Initialize()
{
    WebConfig config = Config::webConfig;

    CommKey = config.CommKey;

    server.on("/", HTTP_GET, HandleRoot);
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

void WebServer::HandleAction(AsyncWebServerRequest *request)
{
    if(!ValidateRequest(request)) return;

    String action = request->arg("action");

    ActionRequestResult actionResult;

    REMOTELOG_D("Received Action HTTP Request (Action: %s)", action.c_str());

    if(action == "unlock") actionResult = ActionHandler::Unlock();
    else if(action == "disengage") actionResult = ActionHandler::Disengage();
    else if(action == "engage") actionResult = ActionHandler::Engage();
    else
    {
        REMOTELOG_W("Invalid action: %s", action.c_str());
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
