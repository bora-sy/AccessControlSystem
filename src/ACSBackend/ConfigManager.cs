using ACSBackend.Database;
using ACSBackend.Database.Models;
using Microsoft.AspNetCore.Components;
using Newtonsoft.Json;

namespace ACSBackend
{

    public enum ConfigEnum
    {
        DISCORD_TOKEN,
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
            return DB.Configs.Find(key)?.VALUE;
        }

        public static string? GetConfig(ConfigEnum c)
        {
            return GetConfig(c.ToString());
        }

        public static void SetConfig(string key, string value)
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

    }

}
