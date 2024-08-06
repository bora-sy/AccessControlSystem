#include "Network/NetworkMain.h"
#define TAG "NetworkMain"

bool NetworkMain::Initialize()
{

    Melody startMelody;
    startMelody.AddNote(1500, 100);
    startMelody.AddRest(50);
    startMelody.AddNote(1500, 100);

    Melody connectedMelody;
    connectedMelody.AddNote(1500, 100);
    connectedMelody.AddRest(50);
    connectedMelody.AddNote(1500, 100);
    connectedMelody.AddRest(50);
    connectedMelody.AddNote(1500, 100);

    Melody failMelody;
    failMelody.AddNote(600, 200);
    failMelody.AddRest(100);
    failMelody.AddNote(600, 200);
    failMelody.AddRest(100);
    failMelody.AddNote(600, 200);


    MelodyPlayer::PlayMelody(startMelody);

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
        MelodyPlayer::PlayMelody(failMelody);
        ESP_LOGE(TAG, "Failed to connect to WiFi");
        return false;
    }

    ESP_LOGI(TAG, "Connected to WiFi (IP Address: %s)", WiFi.localIP().toString().c_str());
    MelodyPlayer::PlayMelody(connectedMelody);
    delay(400);
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