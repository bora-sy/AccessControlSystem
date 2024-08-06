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

        [HttpPost("setdeviceip")]
        public IActionResult SetDeviceIP(string ipaddr)
        {
            if(!IPAddress.TryParse(ipaddr, out IPAddress? IP) || IP == null) return BadRequest("invalid_ip");

            string ipstr = IP.ToString();

            ConfigManager.SetConfig(ConfigEnum.COREDEVICE_IP, ipstr);

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
