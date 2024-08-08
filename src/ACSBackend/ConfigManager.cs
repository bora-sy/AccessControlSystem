using ACSBackend.Database;
using ACSBackend.Database.Models;
using ACSBackend.Utils;
using Microsoft.AspNetCore.Components;
using Newtonsoft.Json;

namespace ACSBackend
{

    public enum ConfigEnum
    {
        COREDEVICE_COMMKEY,
        FRONTDEVICE_COMMKEY,
        COREDEVICE_IP,
        FRONTDEVICE_IP,
        DISCORD_TOKEN,
        REMOTE_LOGGING_PORT,
        REMOTE_LOGGING_IP,
    }

    public static class ConfigManager
    {
        public static string DBCNCSTR => Environment.GetEnvironmentVariable("ACS-DBCNCSTR")!;

        private static AppDBContext DB;

        public static void InitConfigManager(IServiceProvider services)
        {
            DB = services.GetService<AppDBContext>()!;
        }

        private static string? GetConfig(string key)
        {
            string? res = DB.Configs.Find(key)?.VALUE;
            return res;
        }

        public static string? GetConfig(ConfigEnum c)
        {
            return GetConfig(c.ToString());
        }

        private static void SetConfig(string key, string value)
        {
            var config = DB.Configs.Find(key);
            if (config == null)
            {
                DB.Configs.Add(new Config { KEY = key, VALUE = value });
            }
            else
            {
                config.VALUE = value;
            }

            DB.SaveChanges();
        }

        public static void SetConfig(ConfigEnum key, string value)
        {
            SetConfig(key.ToString(), value);
        }

    }

}
