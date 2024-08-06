#include "Network/OTA.h"
#define TAG "OTA"

AsyncWebServer OTA::OTAServer(81);

unsigned long ota_progress_millis = 0;

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}



bool OTA::Initialize()
{
    ESP_LOGI(TAG, "Initializing OTA");

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

    ESP_LOGI(TAG, "OTA Server Initialized (Username: %s / Password: %s)", config.Username, config.Password);

    return true;
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