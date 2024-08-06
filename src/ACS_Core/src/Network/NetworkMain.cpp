#include "Network/NetworkMain.h"
#define TAG "NetworkMain"

Melody NetworkMain::initMelody;
Melody NetworkMain::connectedMelody;
Melody NetworkMain::failedMelody;
Melody NetworkMain::connectionLostMelody;

bool NetworkMain::Initialize()
{

    initMelody.AddNote(1500, 100);
    initMelody.AddRest(50);
    initMelody.AddNote(1500, 100);

    connectedMelody.AddNote(1500, 100);
    connectedMelody.AddRest(50);
    connectedMelody.AddNote(1500, 100);
    connectedMelody.AddRest(50);
    connectedMelody.AddNote(1500, 100);

    failedMelody.AddNote(600, 200);
    failedMelody.AddRest(100);
    failedMelody.AddNote(600, 200);
    failedMelody.AddRest(100);
    failedMelody.AddNote(600, 200);

    connectionLostMelody.AddNote(1500, 100);
    connectionLostMelody.AddRest(50);
    connectionLostMelody.AddNote(1000, 100);
    connectionLostMelody.AddRest(50);
    connectionLostMelody.AddNote(700, 100);

    //--------------------------------------------

    MelodyPlayer::PlayMelody(initMelody);

    WiFiConfig config = Config::wifiConfig;

    WiFi.begin(config.SSID, config.Password);
    ESP_LOGI(TAG, "Connecting to WiFi... (SSID: %s / Password: %s)", config.SSID, config.Password);

    uint32_t start = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - start < WIFI_CONNECTION_TIMEOUT)
    {
        delay(100);
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        MelodyPlayer::PlayMelody(failedMelody);
        ESP_LOGE(TAG, "Failed to connect to WiFi");
        return false;
    }

    ESP_LOGI(TAG, "Connected to WiFi (IP Address: %s)", WiFi.localIP().toString().c_str());
    MelodyPlayer::PlayMelody(connectedMelody);
    delay(400);
    return true;
}

void NetworkMain::CheckWiFiConnection()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        ESP_LOGE(TAG, "WiFi connection lost, reconnecting...");
        MelodyPlayer::PlayMelody(connectionLostMelody);

        WiFiConfig config = Config::wifiConfig;

        WiFi.begin(config.SSID, config.Password);

        uint32_t start = millis();

        while (WiFi.status() != WL_CONNECTED && millis() - start < WIFI_CONNECTION_TIMEOUT)
        {
            delay(100);
        }

        if (WiFi.status() != WL_CONNECTED)
        {
            ESP_LOGE(TAG, "Failed to reconnect to WiFi");
            MelodyPlayer::PlayMelody(failedMelody);
            delay(800);
            ESP.restart();
            return;
        }
    }
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