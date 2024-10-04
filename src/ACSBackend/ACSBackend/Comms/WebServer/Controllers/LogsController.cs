using ACSBackend.Comms.RemoteLogging;
using Microsoft.AspNetCore.Mvc;
using System.Net;

namespace ACSBackend.Comms.WebServer.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class LogsController(ILogger<LogsController> logger, IConfiguration configuration, RemoteLoggingService remoteLogging)
                : ControllerBase
    {
        private readonly IConfiguration _configuration = configuration;
        private readonly ILogger<LogsController> _logger = logger;
        private readonly RemoteLoggingService _remoteLogging = remoteLogging;


        [HttpPost("redirect")]
        public IActionResult SetDeviceIP(string pin, string ip, int port)
        {
            if (!IPAddress.TryParse(ip, out IPAddress? IP) || IP == null) return BadRequest("invalid_ip");

            _remoteLogging.CoreLogger.RedirectLogs(new IPEndPoint(IP, port));

            return Ok("success");
        }
    }
}
