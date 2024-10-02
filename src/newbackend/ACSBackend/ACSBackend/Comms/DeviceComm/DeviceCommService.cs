
using ACSBackend.Configurations;
using Microsoft.Extensions.Options;
using System.Net;

namespace ACSBackend.Comms.DeviceComm
{
    public partial class DeviceCommService : BackgroundService
    {
        ILogger<DeviceCommService> _logger;
        DeviceConfiguration _config;

        DeviceRequestSender CoreReqSender;
        DeviceRequestSender FrontReqSender;

        public CoreComm CoreComm { get; private set; }



        public DeviceCommService(ILogger<DeviceCommService> logger, IOptions<DeviceConfiguration> config, IServiceProvider services)
        {
            _logger = logger;
            _config = config.Value;

            IPAddress CoreIP = IPAddress.Parse(_config.CoreIP);
            IPAddress FrontIP = IPAddress.Parse(_config.FrontIP);

            CoreReqSender = new DeviceRequestSender(_config.CoreCommKey, CoreIP);
            FrontReqSender = new DeviceRequestSender(_config.FrontCommKey, FrontIP);

            CoreComm = new CoreComm(CoreReqSender, services);
        }


        protected override Task ExecuteAsync(CancellationToken stoppingToken)
        {
            return Task.CompletedTask;
        }
    }
}
