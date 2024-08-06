#include "Config.h"
#define KEY "Config"


WiFiConfig Config::wifiConfig;

bool Config::Initialize()
{
    
    if(
    !LoadConfig("/wifi.cfg", &wifiConfig, sizeof(WiFiConfig)) ||
    )
    {
        ESP_LOGE(KEY, "Failed to load config");
        return false;
    }

    return true;
}




bool Config::LoadConfig(const char* path, void* config, size_t size)
{
    if(!DataSaving::FileExists(path))
    {
        ESP_LOGE(KEY, "Config file %s does not exist", path);
        return false;
    }

    if(DataSaving::ReadData(path, (uint8_t*)config, size) == size) return true;
    else
    {
        ESP_LOGE(KEY, "Failed to read config from %s", path);
        return false;
    }

    return true;
}

bool Config::SaveConfig(const char* path, void* config, size_t size)
{
    if(DataSaving::WriteData(path, (uint8_t*)config, size, true) == size) return true;
    else
    {
        ESP_LOGE(KEY, "Failed to write config to %s", path);
        return false;
    }

    return true;
}