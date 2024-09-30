using ACSBackend.Database.Models;

namespace ACSBackend.Comms.DeviceComm
{
    public class CoreComm
    {
        private DeviceRequestSender RequestSender;

        public CoreComm(DeviceRequestSender requestSender)
        {
            RequestSender = requestSender;
        }

        public async Task<ActionRequestResult> ExecuteAction(DeviceAction action, DeviceActionSource source, User? user)
        {
            string path = $"/action?action={(int)action}&actionsource={(int)source}";
            var res = (await RequestSender.POST(path)).ToActionRequestResult() ?? ActionRequestResult.ERROR;
            //await LoggerIntegration.LogActionAsync(action, res, source, user);

            return res;
        }

        public async Task<bool> PingDevice()
        {
            return (await RequestSender.GET("/ping")).IsSuccessStatusCode;
        }

    }
}
