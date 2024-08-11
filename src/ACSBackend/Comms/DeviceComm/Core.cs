using System.Net;

namespace ACSBackend.Comms.DeviceComm
{
    public static partial class DeviceCommMain
    {
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

            private static async Task<HttpStatusCode> GET(string path)
            {
                string url = BuildURL(path);

                HttpClient client = new HttpClient();
                HttpRequestMessage msg = new HttpRequestMessage(HttpMethod.Get, url);
                try
                {
                    msg.Headers.Add("User-Agent", BACKENDUSERAGENT);
                    msg.Headers.Add("CommKey", CoreCommKey);

                    HttpResponseMessage response = await client.SendAsync(msg);
                    HttpStatusCode code = response.StatusCode;

                    response.Dispose();

                    return code;
                }
                catch (Exception)
                {
                    return (HttpStatusCode)(-1);
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

            public static async Task<bool> PingDevice()
            {
                try
                {
                    return await GET("/ping") == HttpStatusCode.OK;
                }
                catch (Exception)
                {
                    return false;
                }
            }
        }
    }
}
