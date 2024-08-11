using ACSBackend.Database;
using System.Net.Http.Headers;
using System.Net;
using System.Text.Json.Nodes;
using System.Text;
using ACSBackend.Utils;

namespace ACSBackend.Comms
{
    public static class DeviceCommMain
    {
        private const string BACKENDUSERAGENT = "ACS_Backend";

        const int DEVICE_PORT = 80;

        private static AppDBContext DB = null!;

        public static string CoreCommKey { get; private set; } = null!;
        public static string FrontCommKey { get; private set; } = null!;

        public static async Task InitDeviceComm(IServiceProvider services)
        {
            DB = services.GetService<AppDBContext>()!;

            CoreCommKey = ConfigManager.GetConfig(ConfigEnum.COREDEVICE_COMMKEY)!;
            FrontCommKey = ConfigManager.GetConfig(ConfigEnum.FRONTDEVICE_COMMKEY)!;

            Permission perms = 0;
            perms.AddPermissions(Permission.USE_ACTIONS, Permission.ACTION_UNLOCK, Permission.ACTION_DISENGAGE, Permission.MANAGE_PERMS);
        }

        public static class Core
        {
            private static string BuildURL(string path) => $"http://{ConfigManager.GetConfig(ConfigEnum.COREDEVICE_IP)}:{DEVICE_PORT}{path}";

            private static async Task<ActionRequestResult> POST(string path)
            {
                string url = BuildURL(path);

                HttpClient client = new HttpClient();
                HttpRequestMessage msg = new HttpRequestMessage(HttpMethod.Post, url);
                try
                {
                    msg.Headers.Add("User-Agent", BACKENDUSERAGENT);
                    msg.Headers.Add("CommKey", CoreCommKey);

                    HttpResponseMessage response = await client.SendAsync(msg);

                    if (response.StatusCode != HttpStatusCode.OK) return ActionRequestResult.ERROR;
                    string res = await response.Content.ReadAsStringAsync();
                    response.Dispose();
                    return Enum.Parse<ActionRequestResult>(res);
                }
                catch (Exception)
                {
                    return ActionRequestResult.ERROR;
                }
                finally
                {
                    if (client != null) client.Dispose();
                    if (msg != null) msg.Dispose();
                }
            }

            public static async Task<ActionRequestResult> ExecuteAction(DeviceAction action, DeviceActionSource source)
            {
                try
                {
                    string path = $"/action?action={(int)action}&actionsource={(int)source}";
                   
                    return await POST(path);
                }
                catch (Exception)
                {
                    return ActionRequestResult.ERROR;
                }
            }
        }





        public enum DeviceAction
        {
            NONE = 0,
            Unlock,
            Engage,
            Disengage
        }

        public enum DeviceActionSource
        {
            NONE = 0,
            Button,
            Front,
            Discord,
            Web
        }

        public enum ActionRequestResult
        {
            ERROR = -1,
            SUCCESS = 0,
            ALREADY_UNLOCKED,
            ALREADY_ENGAGED,
            ALREADY_DISENGAGED
        }
    }
}
