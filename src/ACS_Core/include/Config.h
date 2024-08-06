#pragma once
#include "DataSaving.h"



struct WiFiConfig
{
    char SSID[50];
    char Password[50];

    WiFiConfig(const char* _ssid, const char* _password);
    WiFiConfig();
};


struct OTAConfig
{
    char Username[20];
    char Password[20];

    OTAConfig(const char* _username, const char* _password);
    OTAConfig();
};


struct WebServerConfig
{
    char CommKey[33];

    WebServerConfig(const char* _commKey);
    WebServerConfig();
};



class Config
{
private:
    static bool LoadConfig(const char* path, void* config, size_t size);
    static bool SaveConfig(const char* path, void* config, size_t size);
public:
    static WiFiConfig wifiConfig;
    static OTAConfig otaConfig;
    static WebServerConfig webServerConfig;
    
    static bool Initialize();
};