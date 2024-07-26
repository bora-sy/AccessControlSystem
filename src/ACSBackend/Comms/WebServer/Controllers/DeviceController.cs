using ACSBackend.Comms.WebServer.Filters;
using Microsoft.AspNetCore.Mvc;
using System.Net;

namespace ACSBackend.Comms.WebServer.Controllers
{
    [ApiController]
    [Route("[controller]")]
    [RequireDeviceKey]
    public class DeviceController(ILogger<DeviceController> logger, IConfiguration configuration)
            : ControllerBase
    {
        private readonly IConfiguration _configuration = configuration;
        private readonly ILogger<DeviceController> _logger = logger;

        /// <summary>
        /// Simple ping
        /// </summary>
        /// <returns></returns>
        [HttpGet]
        public IActionResult GetDefault()
        {
            return Ok("pong");
        }

        [HttpPost("setdeviceip")]
        public IActionResult PostDefault(string ipaddr)
        {
            if(!IPAddress.TryParse(ipaddr, out IPAddress? IP) || IP == null) return BadRequest("invalid_ip");

            string ipstr = IP.ToString();

            ConfigManager.SetConfig(ConfigEnum.DEVICE_IP, ipstr);

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
