using ACSBackend.Comms.DeviceComm;
using ACSBackend.Database;
using Microsoft.AspNetCore.Mvc;

namespace ACSBackend.Comms.WebServer.Controllers
{
    /// <summary>
    /// Controller for the users
    /// </summary>
    /// <param name="logger"></param>
    /// <param name="configuration"></param>
    /// <param name="db"></param>
    [ApiController]
    [Route("[controller]")]
    public class Controller(ILogger<Controller> logger, IConfiguration configuration, AppDBContext db, DeviceCommService deviceComm)
            : ControllerBase
    {
        private readonly IConfiguration _configuration = configuration;
        private readonly ILogger<Controller> _logger = logger;
        private readonly AppDBContext _db = db;
        private readonly DeviceCommService _deviceComm = deviceComm;

        /// <summary>
        /// Simple ping
        /// </summary>
        /// <returns></returns>
        [HttpGet]
        public IActionResult GetDefault()
        {
            return Ok("pong");
        }

        [HttpGet("execute")]
        public async Task<IActionResult> ExecuteAction(ulong pin, string action)
        {
            action = action.ToLower();

            DeviceAction deviceAction = action switch
            {
                "unlock" => DeviceAction.Unlock,
                "engage" => DeviceAction.Engage,
                "disengage" => DeviceAction.Disengage,
                _ => (DeviceAction)(-1)
            };

            if (deviceAction == (DeviceAction)(-1)) return BadRequest("Invalid Action");

            var user = _db.Users.Where(x => x.ACSPin == pin).FirstOrDefault();

            if (user == null) return BadRequest("Incorrect PIN");

            var res = await _deviceComm.CoreComm.ExecuteActionAsync(deviceAction, DeviceActionSource.Web, user);


            return res switch
            {
                ActionRequestResult.SUCCESS => Ok("success"),
                ActionRequestResult.ERROR => StatusCode(500, "Error"),
                ActionRequestResult.ALREADY_UNLOCKED => BadRequest("Already Unlocked"),
                ActionRequestResult.ALREADY_ENGAGED => BadRequest("Already Engaged"),
                ActionRequestResult.ALREADY_DISENGAGED => BadRequest("Already Disengaged"),
                _ => StatusCode(500, "Unknown Error")
            };

        }
    }
}
