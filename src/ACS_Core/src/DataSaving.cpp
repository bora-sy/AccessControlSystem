#include "DataSaving.h"
#define TAG "DataSaving"

bool DataSaving::Initialize()
{
    ESP_LOGI(TAG, "Initializing LittleFS");
    bool suc = LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED);
    if (!suc) ESP_LOGE(TAG, "Failed to initialize LittleFS");
    ESP_LOGI(TAG, "LittleFS Initialized");
    return suc;
}

bool DataSaving::FileExists(const char *path)
{
    return LittleFS.exists(path);
}

bool DataSaving::CreateFile(const char *path)
{
    if (FileExists(path))
        return false;

    LittleFS.open(path, "w").close();

    return true;
}

bool DataSaving::DeleteFile(const char *path)
{
    if (!FileExists(path))
        return false;

    return LittleFS.remove(path);
}

size_t DataSaving::WriteData(const char *path, const uint8_t *data, const size_t len, bool createFileIfDoesntExist)
{
    if (!FileExists(path))
    {
        if (!createFileIfDoesntExist || !CreateFile(path))
            return -1;
    }

    File file = LittleFS.open(path, "w");
    if (!file)
    {
        ESP_LOGW(TAG, "Failed to open file \"%s\" for writing", path);
        return -1;
    }

    size_t res = file.write(data, len);

    file.close();

    return res;
}

size_t DataSaving::ReadData(const char *path, uint8_t *data, const size_t len)
{
    if (!FileExists(path))
        return false;

    File file = LittleFS.open(path, "r");
    if (!file)
    {
        ESP_LOGW(TAG, "Failed to open file \"%s\" for reading", path);
        return -1;
    }

    size_t res = file.read(data, len);

    file.close();

    return res;
}