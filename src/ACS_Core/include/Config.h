#pragma once
#include "DataSaving.h"



struct WiFiConfig
{
    char SSID[50];
    char Password[50];

    WiFiConfig(const char* _ssid, const char* _password);
    WiFiConfig();
};



class Config
{
private:
    static bool LoadConfig(const char* path, void* config, size_t size);
    static bool SaveConfig(const char* path, void* config, size_t size);
public:
    static WiFiConfig wifiConfig;
    static bool Initialize();
};