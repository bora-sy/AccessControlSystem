using ACSBackend.Comms.WebServer.Filters;
using Microsoft.AspNetCore.Mvc;
using System.Net;

namespace ACSBackend.Comms.WebServer.Controllers
{
    [ApiController]
    [Route("[controller]")]
    [RequireCoreCommKey]
    public class CoreDeviceController(ILogger<CoreDeviceController> logger, IConfiguration configuration)
            : ControllerBase
    {
        private readonly IConfiguration _configuration = configuration;
        private readonly ILogger<CoreDeviceController> _logger = logger;

        private const string COREDEVICE_USERAGENT = "ACS_CORE";

        [HttpGet("ping")]
        public IActionResult Ping()
        {
            if(Request.Headers.UserAgent == COREDEVICE_USERAGENT)
            {
                string? ipaddr = Request.HttpContext.Connection.RemoteIpAddress!.MapToIPv4().ToString();
                string? oldipaddr = ConfigManager.GetConfig(ConfigEnum.COREDEVICE_IP);

                if(ipaddr != oldipaddr && ipaddr != null)
                {
                    ConfigManager.SetConfig(ConfigEnum.COREDEVICE_IP, ipaddr);
                    Console.WriteLine($"Updated Core Device IP Address ({oldipaddr} ==> {ipaddr})");
                }
            }

            return Ok("success");
        }

        [HttpPost("logalarm")]
        public IActionResult LogAlarm(bool preAlarm)
        {
            Console.WriteLine($"ALARM RECEIVED (PREALARM: {preAlarm})");
            return Ok("TEMP");
        }

    }
}
