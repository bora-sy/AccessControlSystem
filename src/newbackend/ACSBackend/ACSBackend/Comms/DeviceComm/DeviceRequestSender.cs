using System.Net;

namespace ACSBackend.Comms.DeviceComm
{
    public class DeviceRequestSender
    {
        const string BACKENDUSERAGENT = "ACS_Backend";
        const int DEVICE_PORT = 80;

        private string CommKey;
        private IPAddress IP;

        public DeviceRequestSender(string commKey, IPAddress IP)
        {
            this.CommKey = commKey;
            this.IP = IP;
        }


        private string BuildURL(string path) => $"http://{IP}:{DEVICE_PORT}{path}";

        private async Task<RequestResponse> SendAsync(string path, HttpMethod method)
        {
            string url = BuildURL(path);

            HttpClient client = new HttpClient();
            HttpRequestMessage msg = new HttpRequestMessage(method, url);
            try
            {
                msg.Headers.Add("User-Agent", BACKENDUSERAGENT);
                msg.Headers.Add("CommKey", CommKey);

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

        public async Task<RequestResponse> GET(string path)
        {
            return await SendAsync(path, HttpMethod.Get);
        }

        public async Task<RequestResponse> POST(string path)
        {
            return await SendAsync(path, HttpMethod.Post);
        }
    }


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

}
