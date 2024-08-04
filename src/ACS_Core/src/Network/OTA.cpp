#include "Network/OTA.h"
#define TAG "OTA"

AsyncWebServer OTA::OTAServer(81);

bool OTA::Initialize()
{
    OTAConfig config = GetConfig();

    if(strlen(config.Username) == 0)
    {
        ESP_LOGW(TAG, "No OTA config found");
        return false;
    }

    OTAServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(200, "text/plain", "OTA Server");
    });

    ElegantOTA.begin(&OTAServer, config.Username, config.Password);
    OTAServer.begin();

    ESP_LOGI(TAG, "OTA Server Initialized (Username: %s / Password: %s)", config.Username, config.Password);

    return true;
}

void OTA::Loop()
{
    ElegantOTA.loop();
}


OTAConfig OTA::GetConfig()
{
    if(!DataSaving::FileExists("/otaconfig.bin"))
    {
        ESP_LOGW(TAG, "OTA config file not found");
        return OTAConfig();
    }

    OTAConfig config;

    uint16_t readLen = DataSaving::ReadData("/otaconfig.bin", (uint8_t*)&config, sizeof(OTAConfig));

    if(readLen != sizeof(OTAConfig))
    {
        ESP_LOGE(TAG, "Failed to read OTA config file");
        return OTAConfig();
    }

    return config;
}

bool OTA::SetConfig(OTAConfig config)
{
    ESP_LOGI(TAG, "Setting OTA config (Username: %s / Password: %s)", config.Username, config.Password);
    return DataSaving::WriteData("/otaconfig.bin", (uint8_t*)&config, sizeof(OTAConfig), true) == sizeof(OTAConfig);
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