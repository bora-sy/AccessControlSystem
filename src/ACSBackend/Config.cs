using Newtonsoft.Json;

namespace ACSBackend
{
    public static class Config
    {
        public static string ConfigFolder => Environment.GetEnvironmentVariable("ACS-CONFIG_FOLDER")!;

        public static string FileDiscord => Path.Combine(ConfigFolder, "discord.json");
        public static string FileDatabase => Path.Combine(ConfigFolder, "database.json");
        public static string FileComm => Path.Combine(ConfigFolder, "comm.json");

        public static DiscordConfig Discord { get; private set; }
        public static DatabaseConfig Database { get; private set; }
        public static CommConfig Comm { get; private set; }

        private static void InitializeConfig<T>(string configFilepath)
        {
            var prop = typeof(Config).GetProperties().Where(p => p.PropertyType == typeof(T)).FirstOrDefault();
            if (prop == null) throw new InvalidOperationException($"No property of type {typeof(T)} found in Config class.");

            if (!File.Exists(configFilepath))
            {
                var obj = Activator.CreateInstance<T>();
                File.WriteAllText(configFilepath, JsonConvert.SerializeObject(obj, Formatting.Indented));
                throw new InvalidOperationException($"Config file not found. Created a new one at {configFilepath}. Please fill it out and restart the application.");
            }

            var json = File.ReadAllText(configFilepath);

            T? cfg = JsonConvert.DeserializeObject<T>(json);
            if (cfg == null) throw new InvalidOperationException($"Failed to deserialize config file at {configFilepath}.");

            prop.SetValue(null, cfg);
        }

        public static void Initialize()
        {
            InitializeConfig<DiscordConfig>(FileDiscord);
            InitializeConfig<DatabaseConfig>(FileDatabase);
            InitializeConfig<CommConfig>(FileComm);

        }
    }


    public class DiscordConfig
    {

        [JsonProperty("Token")]
        public string Token { get; private set; }
    }

    public class DatabaseConfig
    {
        [JsonProperty("ConnectionString")]
        public string ConnectionString { get; private set; }
    }

    public class CommConfig
    {
        [JsonProperty("TCP_Endpoint")]
        public string TCP_Endpoint { get; private set; }

        [JsonProperty("TCP_AuthKey")]
        public ulong TCP_AuthKey { get; private set; }
    }

}
