﻿using ACSBackend.Configurations;
using Microsoft.Extensions.Options;
using System.Net;

namespace ACSBackend.Comms.RemoteLogging
{
    public class RemoteLoggingService(ILogger<RemoteLoggingService> logger, IOptions<DeviceConfiguration> config) : BackgroundService
    {
        private readonly ILogger<RemoteLoggingService> _logger = logger;
        private readonly DeviceConfiguration _config = config.Value;

        public RemoteLogger CoreLogger { get; private set; }
        public RemoteLogger FrontLogger { get; private set; }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {

            _logger.LogInformation($"REMOTE LOGGING SERVICE EXECUTEASYNC STARTED / Hash Code: {this.GetHashCode()}");

            CoreLogger = new RemoteLogger(IPAddress.Any, _config.CoreLoggingPort);

            _logger.LogInformation($"CREATED CORELOGGER INFO (CORELOGGER == NULL: {CoreLogger == null})");

            //FrontLogger = new RemoteLogger(IPAddress.Any, _config.FrontLoggingPort);

            _ = Task.Run(async delegate ()
            {
                while (!stoppingToken.IsCancellationRequested)
                {
                    try
                    {
                        await CoreLogger.ReceiveAsync(stoppingToken);
                    }
                    catch (Exception ex)
                    {
                        if (!stoppingToken.IsCancellationRequested)
                            _logger.LogError(ex, "An error occurred while receiving a log from the core device");
                    }
                }
            });
            /*
            _ = Task.Run(async delegate ()
            {
                while (!stoppingToken.IsCancellationRequested)
                {
                    try
                    {
                        await FrontLogger.ReceiveAsync(stoppingToken);
                    }
                    catch (Exception ex)
                    {
                        if (!stoppingToken.IsCancellationRequested)
                            _logger.LogError(ex, "An error occurred while receiving a log from the front device");
                    }
                }
            });
            */

        }


    }
}
