using ACSBackend.Comms.RemoteLogging;
using ACSBackend.Comms.WebServer.Filters;
using ACSBackend.Utils;
using Microsoft.AspNetCore.Mvc;
using System.Net;

namespace ACSBackend.Comms.WebServer.Controllers
{
    [ApiController]
    [Route("[controller]")]
    [RequirePermission(Permission.MANAGE_LOGS)]
    public class LogsController(ILogger<LogsController> logger, IConfiguration configuration)
                : ControllerBase
    {
        private readonly IConfiguration _configuration = configuration;
        private readonly ILogger<LogsController> _logger = logger;


        [HttpPost("redirect")]
        public IActionResult SetDeviceIP(string pin, string ip, int port)
        {
            if (!IPAddress.TryParse(ip, out IPAddress? IP) || IP == null) return BadRequest("invalid_ip");

            RemoteLoggingMain.RedirectLogs(IP, port);

            return Ok("success");
        }
    }
}
