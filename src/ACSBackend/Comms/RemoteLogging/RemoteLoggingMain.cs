
using ACSBackend.Database.Models;
using System.Diagnostics;
using System.Net;

namespace ACSBackend.Comms.RemoteLogging
{
    public static class RemoteLoggingMain
    {
        private static RemoteLogger logger;

        private static StreamWriter sw;

        public static async Task Initialize()
        {
            IPAddress IP = IPAddress.Parse(ConfigManager.GetConfig(ConfigEnum.REMOTE_LOGGING_IP)!);

            int port = int.Parse(ConfigManager.GetConfig(ConfigEnum.REMOTE_LOGGING_PORT)!);

            logger = new RemoteLogger(IP, port);
            logger.OnLog += Logger_OnLog;
        }

        private static void Logger_OnLog(RemoteLog obj)
        {
            sw.WriteLine($"[{obj.Timestamp}] [{obj.Severity}] {obj.Message}");
        }
    }
}
