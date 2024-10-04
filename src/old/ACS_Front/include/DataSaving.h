#pragma once
#include <LittleFS.h>
#include "esp_log.h"

#define FORMAT_LITTLEFS_IF_FAILED 1

class DataSaving
{
    public:
    static bool Initialize();

    static bool FileExists(const char* path);
    static bool CreateFile(const char* path);
    static bool DeleteFile(const char* path);
    static size_t WriteData(const char* path, const uint8_t* data, const size_t len, bool createFileIfDoesntExist = true);
    static size_t ReadData(const char* path, uint8_t* data, const size_t len);
};