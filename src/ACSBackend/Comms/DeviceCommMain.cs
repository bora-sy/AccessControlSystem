using ACSBackend.Database;
using System.Net.Http.Headers;
using System.Net;
using System.Text.Json.Nodes;
using System.Text;

namespace ACSBackend.Comms
{
    public static class DeviceCommMain
    {
        const int DEVICE_PORT = 80;

        private static AppDBContext DB = null!;
        public static string DeviceAPIKey { get; private set; } = null!;

        public static async Task InitDeviceComm(IServiceProvider services)
        {
            DB = services.GetService<AppDBContext>()!;

            DeviceAPIKey = ConfigManager.GetConfig(ConfigEnum.DEVICE_APIKEY)!;
        }

        private static string BuildURL(string path) => $"http://{ConfigManager.GetConfig(ConfigEnum.COREDEVICE_IP)}:{DEVICE_PORT}{path}";

        private static async Task<ActionRequestResult> POST(string path)
        {
            string url = BuildURL(path);

            HttpClient client = new HttpClient();
            HttpRequestMessage msg = new HttpRequestMessage(HttpMethod.Post, url);
            try
            {
                msg.Headers.Authorization = new AuthenticationHeaderValue("", DeviceAPIKey);

                HttpResponseMessage response = await client.SendAsync(msg);

                if(response.StatusCode != HttpStatusCode.OK) return ActionRequestResult.ERROR;
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

        public static async Task<ActionRequestResult> ExecuteAction(DeviceAction action)
        {
            try
            {
                string path = action switch
                {
                    DeviceAction.Unlock => "/action/",
                    DeviceAction.Engage => "/engage",
                    DeviceAction.Disengage => "/disengage",
                    _ => throw new NotImplementedException()
                };

                return await POST(path);
            }
            catch (Exception)
            {
                return ActionRequestResult.ERROR;
            }
        }



        public enum DeviceAction
        {
            Unlock,
            Engage,
            Disengage
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
