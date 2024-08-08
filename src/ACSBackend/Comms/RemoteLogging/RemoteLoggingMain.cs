
using ACSBackend.Database.Models;
using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace ACSBackend.Comms.RemoteLogging
{
    public static class RemoteLoggingMain
    {
        private static RemoteLogger logger;

        private static StreamWriter sw;


        private static UdpClient? RedirectClient = null;

        private static UdpClient udp;

        public static async Task Initialize()
        {

            IPAddress IP = IPAddress.Parse(ConfigManager.GetConfig(ConfigEnum.REMOTE_LOGGING_IP)!);

            int port = int.Parse(ConfigManager.GetConfig(ConfigEnum.REMOTE_LOGGING_PORT)!);

            logger = new RemoteLogger(IP, port);
            logger.OnLog += Logger_OnLog;
        }


        public static void RedirectLogs(IPAddress ip, int port)
        {
            CancelRedirect();
            RedirectClient = new UdpClient();
            RedirectClient.Connect(ip, port);
            Console.WriteLine("Redirecting Logs");
        }

        public static void CancelRedirect()
        {
            if(RedirectClient != null)
            {
                try
                {
                    RedirectClient.Close();
                }
                finally
                {
                    RedirectClient.Dispose();
                    RedirectClient = null;
                }

                Console.WriteLine("Redirecting Cancelled");
            }
        }

        private static void Logger_OnLog(RemoteLog obj)
        {
            if(RedirectClient != null)
            {
                RedirectClient.Send(obj.Original, obj.Original.Length);
            }
        }
    }
}
