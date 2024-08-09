#pragma once
#include "DataSaving.h"
#include "IO/MelodyPlayer.h"



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


struct WebConfig
{
    char CommKey[33];
    uint8_t ServerIP[4];


    WebConfig(const char* _commKey, uint8_t serverIP[4]);
    WebConfig();
};



class Config
{
private:
    static bool LoadConfig(const char* path, void* config, size_t size);
    static bool SaveConfig(const char* path, void* config, size_t size);
public:
    static WiFiConfig wifiConfig;
    static OTAConfig otaConfig;
    static WebConfig webConfig;
    const static uint8_t ServerIPAddress[];
    
    static bool Initialize();
};