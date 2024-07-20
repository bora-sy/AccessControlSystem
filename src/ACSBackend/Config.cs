namespace ACSBackend
{
    public static class Config
    {
        public static string? DB_CNCSTR => Environment.GetEnvironmentVariable("ACS-DB_CONNECTION_STRING");

        public static string? DC_TOKEN => Environment.GetEnvironmentVariable("ACS-DISCORD_TOKEN");
    }
}
