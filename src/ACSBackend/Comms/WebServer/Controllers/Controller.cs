using Microsoft.AspNetCore.Mvc;

namespace ACSBackend.Comms.Web.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class Controller(ILogger<Controller> logger, IConfiguration configuration)
            : ControllerBase
    {
        private readonly IConfiguration _configuration = configuration;
        private readonly ILogger<Controller> _logger = logger;

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
