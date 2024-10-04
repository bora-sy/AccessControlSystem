using System.Net;

namespace ACSBackend.Comms.DeviceComm
{
    public static partial class DeviceCommMain
    {
        public static class RequestSender
        {
            public class RequestResponse
            {
                public HttpResponseMessage? ResponseMessage { get; private set; } = null;

                public bool SendSuccess => ResponseMessage != null;

                public HttpStatusCode? statusCode => ResponseMessage == null ? null : ResponseMessage.StatusCode;

                public string? StringContent => ResponseMessage == null ? null : ResponseMessage.Content.ReadAsStringAsync().Result;

                public bool IsSuccessStatusCode => statusCode == HttpStatusCode.OK;

                public ActionRequestResult? ToActionRequestResult()
                {
                    string? str = StringContent;
                    if (str == null) return null;

                    return Enum.Parse<ActionRequestResult>(str);
                }

                public RequestResponse(HttpResponseMessage responseMessage)
                {
                    this.ResponseMessage = responseMessage;
                }

                ~RequestResponse()
                {
                    if (ResponseMessage != null) ResponseMessage.Dispose();
                }

                public RequestResponse() { }


            }

            private const string BACKENDUSERAGENT = "ACS_Backend";

            const int DEVICE_PORT = 80;

            private static string BuildURL(string path, DeviceType device) => $"http://{ConfigManager.GetConfig(device == DeviceType.CORE ? ConfigEnum.COREDEVICE_IP : ConfigEnum.FRONTDEVICE_IP)}:{DEVICE_PORT}{path}";

            private static async Task<RequestResponse> SendAsync(string path, DeviceType device, HttpMethod method)
            {
                string url = BuildURL(path, device);

                HttpClient client = new HttpClient();
                HttpRequestMessage msg = new HttpRequestMessage(method, url);
                try
                {
                    msg.Headers.Add("User-Agent", BACKENDUSERAGENT);
                    msg.Headers.Add("CommKey", device == DeviceType.CORE ? CoreCommKey : FrontCommKey);

                    HttpResponseMessage response = await client.SendAsync(msg);

                    return new RequestResponse(response);
                }
                catch (Exception)
                {
                    return new RequestResponse();
                }
                finally
                {
                    if (client != null) client.Dispose();
                    if (msg != null) msg.Dispose();
                }

            }

            public static async Task<RequestResponse> GET(string path, DeviceType device)
            {
                return await SendAsync(path, device, HttpMethod.Get);
            }

            public static async Task<RequestResponse> POST(string path, DeviceType device)
            {
                return await SendAsync(path, device, HttpMethod.Post);
            }
        }

    }
}
