using ACSBackend.Database.Models;
using ACSBackend.Misc;

namespace ACSBackend.Comms.DeviceComm
{
    public class CoreComm
    {
        private DeviceRequestSender RequestSender;
        private IServiceProvider Services;

        public CoreComm(DeviceRequestSender requestSender, IServiceProvider services)
        {
            RequestSender = requestSender;
            Services = services;
        }

        public async Task<ActionRequestResult> ExecuteActionAsync(DeviceAction action, DeviceActionSource source, User? user)
        {
            using var scope = Services.CreateScope();
            var actionLogger = scope.ServiceProvider.GetRequiredService<ActionLoggerService>();

            string path = $"/action?action={(int)action}&actionsource={(int)source}";
            var res = (await RequestSender.POST(path, TimeSpan.FromSeconds(5))).ToActionRequestResult() ?? ActionRequestResult.ERROR;

            _ = actionLogger.LogActionAsync(source, action, res, user);

            return res;
        }

        public async Task<bool> PingDevice()
        {
            return (await RequestSender.GET("/ping", TimeSpan.FromSeconds(5))).IsSuccessStatusCode;
        }

    }
}
