using ACSBackend.Comms.WebServer.Filters;
using Microsoft.AspNetCore.Mvc;
using System.Net;

namespace ACSBackend.Comms.WebServer.Controllers
{
    [ApiController]
    [Route("[controller]")]
    [RequireFrontCommKey]
    public class FrontDeviceController(ILogger<FrontDeviceController> logger, IConfiguration configuration)
            : ControllerBase
    {
        private readonly IConfiguration _configuration = configuration;
        private readonly ILogger<FrontDeviceController> _logger = logger;

        private const string FRONTDEVICE_USERAGENT = "ACS_FRONT";

        [HttpGet("ping")]
        public IActionResult Ping()
        {
            if (Request.Headers.UserAgent == FRONTDEVICE_USERAGENT)
            {
                string? ipaddr = Request.HttpContext.Connection.RemoteIpAddress!.MapToIPv4().ToString();
                string? oldipaddr = ConfigManager.GetConfig(ConfigEnum.FRONTDEVICE_IP);

                if (ipaddr != oldipaddr && ipaddr != null)
                {
                    ConfigManager.SetConfig(ConfigEnum.FRONTDEVICE_COMMKEY, ipaddr);
                    Console.WriteLine($"Updated Front Device IP Address ({oldipaddr} ==> {ipaddr})");
                }
            }

            return Ok("success");
        }

        [HttpPost("input/keypad")]
        public IActionResult KeypadInput()
        {
            return Ok("test");
        }

        [HttpPost("input/card")]
        public IActionResult CardInput()
        {
            return Ok("test");
        }
    }
}
