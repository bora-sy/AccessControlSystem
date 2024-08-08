#pragma once
#include <AsyncUDP.h>
#include "esp_log.h"
#include "Config.h"

class RemoteLogging
{
private:
    static AsyncUDP udp;
    static void log(esp_log_level_t level, const char* tag, const char* format, ...);
public:
    static void Initialize();
};