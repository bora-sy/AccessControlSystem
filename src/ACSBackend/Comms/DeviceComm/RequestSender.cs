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

                public ActionRequestResult ToActionRequestResult();

                public RequestResponse(HttpResponseMessage responseMessage)
                {
                    this.ResponseMessage = responseMessage;
                }

                ~RequestResponse()
                {
                    if(ResponseMessage != null) ResponseMessage.Dispose();
                }

                public RequestResponse() { }


            }
        }
    }
}
