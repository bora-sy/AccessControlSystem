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
        private static string DeviceAPIKey = null!;

        public static async Task InitDeviceComm(IServiceProvider services)
        {
            DB = services.GetService<AppDBContext>()!;

            DeviceAPIKey = ConfigManager.GetConfig(ConfigEnum.DEVICE_APIKEY)!;
        }

        private static string BuildURL(string path) => $"http://{ConfigManager.GetConfig(ConfigEnum.DEVICE_IP)}:{DEVICE_PORT}{path}";

        private static async Task<ActionResponse> POST(string path)
        {
            string url = BuildURL(path);

            HttpClient client = new HttpClient();
            HttpRequestMessage msg = new HttpRequestMessage(HttpMethod.Post, url);
            try
            {
                msg.Headers.Authorization = new AuthenticationHeaderValue("Bearer", DeviceAPIKey);

                HttpResponseMessage response = await client.SendAsync(msg);

                if(response.StatusCode != HttpStatusCode.OK) return ActionResponse.Error;
                string res = await response.Content.ReadAsStringAsync();
                response.Dispose();
                return Enum.Parse<ActionResponse>(res);
            }
            catch (Exception)
            {
                return ActionResponse.Error;
            }
            finally
            {
                if (client != null) client.Dispose();
                if (msg != null) msg.Dispose();
            }
        }

        public static async Task<ActionResponse> ExecuteAction(Action action)
        {
            try
            {
                string path = action switch
                {
                    Action.Lock => "/lock",
                    Action.Unlock => "/unlock",
                    Action.Engage => "/engage",
                    Action.Disengage => "/disengage",
                    _ => throw new NotImplementedException()
                };

                return await POST(path);
            }
            catch (Exception)
            {
                return ActionResponse.Error;
            }
        }



        public enum Action
        {
            Lock,
            Unlock,
            Engage,
            Disengage,
        }

        public enum ActionResponse
        {
            Error,
            Success,
            Busy,
            NotLocked,
            NotUnlocked,
            NotEngaged,
            NotDisengaged,
        }
    }
}
