#include "Config.h"
#define TAG "Config"

WiFiConfig Config::wifiConfig;
OTAConfig Config::otaConfig;
WebServerConfig Config::webServerConfig;

bool Config::Initialize()
{
    if (
        !LoadConfig("/wifi.cfg", &wifiConfig, sizeof(WiFiConfig)) ||
        !LoadConfig("/ota.cfg", &otaConfig, sizeof(OTAConfig)) ||
        !LoadConfig("/webserver.cfg", &webServerConfig, sizeof(WebServerConfig)))
    {

        Melody failMelody;
        failMelody.AddNote(600, 200);
        failMelody.AddRest(100);
        failMelody.AddNote(600, 200);

        MelodyPlayer::PlayMelody(failMelody);

        ESP_LOGE(TAG, "Failed to load config");
        return false;
    }

    ESP_LOGI(TAG, "WEBSERVER %s", webServerConfig.CommKey);
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