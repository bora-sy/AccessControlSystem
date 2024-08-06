#include "Network/NetworkMain.h"
#define TAG "NetworkMain"

bool NetworkMain::Initialize()
{
    WiFiConfig config = Config::wifiConfig;


    WiFi.begin(config.SSID, config.Password);
    ESP_LOGI(TAG, "Connecting to WiFi... (SSID: %s / Password: %s)", config.SSID, config.Password);

    uint32_t start = millis();

    while(WiFi.status() != WL_CONNECTED && millis() - start < WIFI_CONNECTION_TIMEOUT)
    {
        delay(100);
    }

    if(WiFi.status() != WL_CONNECTED)
    {
        ESP_LOGE(TAG, "Failed to connect to WiFi");
        return false;
    }

    ESP_LOGI(TAG, "Connected to WiFi (IP Address: %s)", WiFi.localIP().toString().c_str());
    return true;
}

WiFiConfig::WiFiConfig(const char *_ssid, const char *_password)
{
    memset(SSID, 0, 50);
    memset(Password, 0, 50);

    strcpy(SSID, _ssid);
    strcpy(Password, _password);
}

WiFiConfig::WiFiConfig()
{
    memset(SSID, 0, 50);
    memset(Password, 0, 50);
}