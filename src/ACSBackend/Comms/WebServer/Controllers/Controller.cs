using ACSBackend.Database;
using ACSBackend.Utils;
using Microsoft.AspNetCore.Mvc;
using static ACSBackend.Comms.DeviceCommMain;

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
    public class Controller(ILogger<Controller> logger, IConfiguration configuration, AppDBContext db)
            : ControllerBase
    {
        private readonly IConfiguration _configuration = configuration;
        private readonly ILogger<Controller> _logger = logger;
        private readonly AppDBContext _db = db;

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

            if (!user.HasPermission(Permission.USE_ACTIONS)) return Unauthorized("No Permission (0)");

            if ((deviceAction == DeviceAction.Unlock) && !user.HasPermission(Permission.ACTION_LOCKUNLOCK)) return Unauthorized("No Permission (1)");
            if ((deviceAction == DeviceAction.Engage || deviceAction == DeviceAction.Disengage) && !user.HasPermission(Permission.ACTION_DISENGAGE)) return Unauthorized("No Permission (2)");

            var res = await Core.ExecuteAction(deviceAction);

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
