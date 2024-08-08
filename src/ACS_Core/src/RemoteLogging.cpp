#include "RemoteLogging.h"

AsyncUDP* RemoteLogging::udp;

bool RemoteLogging::Initialize()
{
    ESP_LOGI("RemoteLogging", "Initializing RemoteLogging");
    udp = new AsyncUDP();
    bool udpsuc = udp->connect(IPAddress(Config::ServerIPAddress), 1256);
    if(udpsuc)
    {
        ESP_LOGI("RemoteLogging", "RemoteLogging Initialized");
    }
    else ESP_LOGE("RemoteLogging", "RemoteLogging Failed to Initialize");

    return udpsuc;
}

void RemoteLogging::Log(const char* format, const char* filename, uint32_t lineNum,  esp_log_level_t level, ...) {
    
    char* text = RemoteLogging::FormatString(format);
    if (text == nullptr) return;

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
}

char* RemoteLogging::FormatString(const char* format, ...) {
    va_list args;
    va_start(args, format);

    int size = vsnprintf(nullptr, 0, format, args) + 1; 
    va_end(args);

    char* buffer = (char*)malloc(size);
    if (!buffer) {
        return nullptr;
    }

    va_start(args, format);
    vsnprintf(buffer, size, format, args);
    va_end(args);

    return buffer;
}