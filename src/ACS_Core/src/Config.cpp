#include "Config.h"
#define TAG "Config"

WiFiConfig Config::wifiConfig;
OTAConfig Config::otaConfig;
WebConfig Config::webConfig;

bool Config::Initialize()
{
    if (
        !LoadConfig(CONFIGPATH_WIFI, &wifiConfig, sizeof(WiFiConfig)) ||
        !LoadConfig(CONFIGPATH_OTA, &otaConfig, sizeof(OTAConfig)) ||
        !LoadConfig(CONFIGPATH_WEB, &webConfig, sizeof(WebConfig)))
    {

        Melody failMelody;
        failMelody.AddTone(600, 200);
        failMelody.AddRest(100);
        failMelody.AddTone(600, 200);

        MelodyPlayer::PlayMelody(failMelody);

        ESP_LOGE(TAG, "Failed to load config");
        return false;
    }

    return true;
}

bool Config::LoadConfig(const char *path, void *config, size_t size)
{
    if (!DataSaving::FileExists(path))
    {
        ESP_LOGE(TAG, "Config file %s does not exist", path);
        return false;
    }

    if (DataSaving::ReadData(path, (uint8_t *)config, size) == size)
        return true;
    else
    {
        ESP_LOGE(TAG, "Failed to read config from %s", path);
        return false;
    }

    return true;
}

bool Config::SaveConfig(const char *path, void *config, size_t size)
{
    if (DataSaving::WriteData(path, (uint8_t *)config, size, true) == size)
        return true;
    else
    {
        ESP_LOGE(TAG, "Failed to write config to %s", path);
        return false;
    }

    return true;
}

bool Config::UpdateWebConfig()
{
    return SaveConfig(CONFIGPATH_WEB, &webConfig, sizeof(WebConfig));
}

//-----------------------------------------------------------

// WEBCOONFIG


WebConfig::WebConfig(const char *_commKey, uint8_t serverIP[4])
{
    memset(CommKey, 0, 33);
    

    strcpy(CommKey, _commKey);
    memcpy(ServerIP, serverIP, 4);
}

WebConfig::WebConfig()
{
    memset(CommKey, 0, 33);
}