#pragma once
#include <AsyncUDP.h>
#include "esp_log.h"
#include "Config.h"

#define REMOTELOG_V(format, ...) RemoteLogging::Log(format,pathToFileName(__FILE__), __LINE__, ESP_LOG_VERBOSE, ##__VA_ARGS__)
#define REMOTELOG_D(format, ...) RemoteLogging::Log(format,pathToFileName(__FILE__), __LINE__, ESP_LOG_DEBUG, ##__VA_ARGS__)
#define REMOTELOG_I(format, ...) RemoteLogging::Log(format,pathToFileName(__FILE__), __LINE__, ESP_LOG_INFO, ##__VA_ARGS__)
#define REMOTELOG_W(format, ...) RemoteLogging::Log(format,pathToFileName(__FILE__), __LINE__, ESP_LOG_WARN, ##__VA_ARGS__)
#define REMOTELOG_E(format, ...) RemoteLogging::Log(format,pathToFileName(__FILE__), __LINE__, ESP_LOG_ERROR, ##__VA_ARGS__)

#define UDPLOGGINGPORT 1357
class RemoteLogging
{
private:
    static AsyncUDP* udp;
    static bool Initialized;
    static void ValidateUDPConnection();
    static uint8_t ConnectedIP[];


public:
    static String FormatString(const char* format, ...);
    static bool Initialize();
    static void Log(const char* format, const char* filename, uint32_t lineNum,  esp_log_level_t level, ...);
};