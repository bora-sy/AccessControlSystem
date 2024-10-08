#include "RemoteLogging.h"

AsyncUDP* RemoteLogging::udp;
bool RemoteLogging::Initialized = false;
uint8_t RemoteLogging::ConnectedIP[4] = {0,0,0,0};

bool RemoteLogging::Initialize()
{
    ESP_LOGI("RemoteLogging", "Initializing RemoteLogging");
    udp = new AsyncUDP();

    bool udpsuc = udp->connect(IPAddress(Config::webConfig.ServerIP), UDPLOGGINGPORT);

    memcpy(ConnectedIP, Config::webConfig.ServerIP, 4);
    

    if(udpsuc)
    {
        Initialized = true;
        ESP_LOGI("RemoteLogging", "RemoteLogging Initialized");
    }
    else ESP_LOGE("RemoteLogging", "RemoteLogging Failed to Initialize");

    return udpsuc;
}

void RemoteLogging::Log(const char* format, const char* filename, uint32_t lineNum,  esp_log_level_t level, ...) {
    if(!Initialized) return;
    ValidateUDPConnection();

    va_list args;
    va_start(args, level);

    int size = vsnprintf(nullptr, 0, format, args) + 1; 
    va_end(args);

    char text[size];

    va_start(args, level);
    vsnprintf(&text[0], size, format, args);
    va_end(args);    

    uint16_t bufLength = 1 + 8 + 4 + strlen(text) + 1 + strlen(filename) + 1;

    uint8_t buf[bufLength];
    memset(buf, 0, bufLength);

    buf[0] = level;
    uint64_t time = millis();
    memcpy(buf + 1, &time, 8);
    memcpy(buf + 9, &lineNum, 4);
    memcpy(buf + 13, text, strlen(text));
    memcpy(buf + 13 + strlen(text) + 1, filename, strlen(filename));

    udp->write(buf, bufLength);

    //if(level <= 4)
    Serial.printf("LOG: %s\n", text);
    

}

String RemoteLogging::FormatString(const char* format, ...) {
    va_list args;
    va_start(args, format);

    int size = vsnprintf(nullptr, 0, format, args) + 1; 
    va_end(args);

    char buffer[size];

    va_start(args, format);
    vsnprintf(buffer, size, format, args);
    va_end(args);

    return String(buffer);
}

void RemoteLogging::ValidateUDPConnection()
{
    if(memcmp(ConnectedIP, Config::webConfig.ServerIP, 4) == 0 && udp->connected())
    {
        Serial.println("UDP connected & IP matches");
        return;
    }

    udp->close();
    Serial.println("UDP closed");

    udp->connect(IPAddress(Config::webConfig.ServerIP), UDPLOGGINGPORT);
    Serial.println("UDP connected");

    memcpy(ConnectedIP, Config::webConfig.ServerIP, 4);
    Serial.println("IP copied");

}