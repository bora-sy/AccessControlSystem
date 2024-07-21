using ACSBackend.Comms.TCP;
using Microsoft.AspNetCore.Mvc;

namespace ACSBackend.Comms.Web.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class Controller(ILogger<Controller> logger, IConfiguration configuration, TCPService s)
            : ControllerBase
    {
        private readonly IConfiguration _configuration = configuration;
        private readonly ILogger<Controller> _logger = logger;
        private readonly TCPService _tcpService = s;

        /// <summary>
        /// Simple ping
        /// </summary>
        /// <returns></returns>
        [HttpGet]
        public IActionResult GetDefault()
        {
            return Ok("pong");
        }
    }
}
