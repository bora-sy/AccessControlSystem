#include "Network/OTA.h"
#define TAG "OTA"

AsyncWebServer OTA::OTAServer(81);

unsigned long ota_progress_millis = 0;

void OTA::onOTAStart() {
  REMOTELOG_I("OTA update started");
}

void OTA::onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    REMOTELOG_I("OTA Progress Current: %u bytes, Final: %u bytes", current, final);
  }
}

void OTA::onOTAEnd(bool success) {

  if(success) REMOTELOG_I("OTA update finished successfully");
  else REMOTELOG_E("There was an error during OTA update");
}



void OTA::Initialize()
{
    OTAConfig config = Config::otaConfig;
    
    OTAServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(200, "text/plain", "OTA Server");
    });

    ElegantOTA.begin(&OTAServer, config.Username, config.Password);

    ElegantOTA.onStart(onOTAStart);
    ElegantOTA.onProgress(onOTAProgress);
    ElegantOTA.onEnd(onOTAEnd);

    OTAServer.begin();

    REMOTELOG_I(TAG, "OTA Server Initialized (Username: %s / Password: %s)", config.Username, config.Password);
}

void OTA::Loop()
{
    ElegantOTA.loop();
}

OTAConfig::OTAConfig(const char *_username, const char *_password)
{
    memset(Username, 0, 20);
    memset(Password, 0, 20);

    strcpy(Username, _username);
    strcpy(Password, _password);
}

OTAConfig::OTAConfig()
{
    memset(Username, 0, 20);
    memset(Password, 0, 20);
}