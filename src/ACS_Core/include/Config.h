#pragma once
#include "DataSaving.h"
#include "IO/MelodyPlayer.h"

#define CONFIGPATH_WIFI "/wifi2.cfg"
#define CONFIGPATH_OTA  "/ota.cfg"
#define CONFIGPATH_WEB  "/web.cfg"


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

    static bool SetWebConfig(const char* commKey, uint8_t ipb1, uint8_t ipb2, uint8_t ipb3, uint8_t ipb4);
    static bool SetWiFiConfig(const char* ssid, const char* password);

public:
    static WiFiConfig wifiConfig;
    static OTAConfig otaConfig;
    static WebConfig webConfig;
    
    static bool Initialize();

    static bool UpdateWebConfig();
    
};