using ACSBackend.Comms.WebServer.Controllers.Filters;
using Microsoft.AspNetCore.Mvc;

namespace ACSBackend.Comms.WebServer.Controllers.Device
{
    [ApiController]
    [Route("[controller]")]
    [RequireCommKey(DeviceComm.DeviceType.CORE)]
    public class CoreController(ILogger<CoreController> logger, IConfiguration configuration)
                : ControllerBase
    {
        private readonly IConfiguration _configuration = configuration;
        private readonly ILogger<CoreController> _logger = logger;

        private const string COREDEVICE_USERAGENT = "ACS_CORE";

        [HttpGet("ping")]
        public IActionResult Ping()
        {
            _logger.LogDebug("Ping received from Core Device");

            if (Request.Headers.UserAgent == COREDEVICE_USERAGENT)
            {
                /*
                string? ipaddr = Request.HttpContext.Connection.RemoteIpAddress!.MapToIPv4().ToString();
                string? oldipaddr = ConfigManager.GetConfig(ConfigEnum.COREDEVICE_IP);

                if (ipaddr != oldipaddr && ipaddr != null)
                {
                    ConfigManager.SetConfig(ConfigEnum.COREDEVICE_IP, ipaddr);
                    Console.WriteLine($"Updated Core Device IP Address ({oldipaddr} ==> {ipaddr})");
                }
                */
            }
            else _logger.LogWarning("Core device User-Agent mismatch");

            return Ok("success");
        }

        [HttpPost("logalarm")]
        public IActionResult LogAlarm(bool prealarm, bool status)
        {
            _logger.LogInformation("Alarm logged from Core Device");
            //LogAlarm(prealarm, status);

            return Ok();
        }

    }
}
