using System.Net;

namespace ACSBackend.Comms.DeviceComm
{
    public static partial class DeviceCommMain
    {
        public static class Core
        {
            public static async Task<ActionRequestResult> ExecuteAction(DeviceAction action, DeviceActionSource source)
            {
                try
                {
                    string path = $"/action?action={(int)action}&actionsource={(int)source}";

                    return (await RequestSender.POST(path, DeviceType.CORE)).ToActionRequestResult() ?? ActionRequestResult.ERROR;
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
                    return (await RequestSender.GET("/ping", DeviceType.CORE)).IsSuccessStatusCode;
                }
                catch (Exception)
                {
                    return false;
                }
            }
        }
    }
}
