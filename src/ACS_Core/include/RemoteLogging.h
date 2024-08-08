#pragma once
#include <AsyncUDP.h>
#include "esp_log.h"
#include "Config.h"

#define REMOTELOG(format, level, ...) RemoteLogging::Log(format, level, ##__VA_ARGS__)
#define REMOTELOG(format) RemoteLogging::Log(format, ESP_LOG_INFO)

class RemoteLogging
{
private:
    static AsyncUDP* udp;

    static char* FormatString(const char* format, ...);

public:
    static bool Initialize();
    static void Log(const char* format, esp_log_level_t level = ESP_LOG_INFO, ...);
};