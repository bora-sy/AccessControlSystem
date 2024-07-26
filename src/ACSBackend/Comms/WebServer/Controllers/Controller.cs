﻿using ACSBackend.Database;
using ACSBackend.Utils;
using Microsoft.AspNetCore.Mvc;
using static ACSBackend.Comms.DeviceCommMain;

namespace ACSBackend.Comms.WebServer.Controllers
{
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
                "lock" => DeviceAction.Lock,
                "unlock" => DeviceAction.Unlock,
                "engage" => DeviceAction.Engage,
                "disengage" => DeviceAction.Disengage,
                _ => (DeviceAction)100
            };

            if (deviceAction == (DeviceAction)100) return BadRequest("Invalid Action");

            var user = _db.Users.Where(x => x.ACSPin == pin).FirstOrDefault();

            if (user == null) return BadRequest("Incorrect PIN");

            if (!user.HasPermission(Permission.USE_ACTIONS)) return Unauthorized("No Permission (0)");

            if ((deviceAction == DeviceAction.Lock || deviceAction == DeviceAction.Unlock) && !user.HasPermission(Permission.ACTION_LOCKUNLOCK)) return Unauthorized("No Permission (1)");
            if ((deviceAction == DeviceAction.Engage || deviceAction == DeviceAction.Disengage) && !user.HasPermission(Permission.ACTION_DISENGAGE)) return Unauthorized("No Permission (2)");

            var res = await DeviceCommMain.ExecuteAction(deviceAction);

            return res switch
            {
                DeviceActionResponse.Success => Ok("success"),
                DeviceActionResponse.Busy => BadRequest("busy"),
                DeviceActionResponse.NotLocked => BadRequest("Not Locked"),
                DeviceActionResponse.NotUnlocked => BadRequest("Not Unlocked"),
                DeviceActionResponse.NotEngaged => BadRequest("Not Engaged"),
                DeviceActionResponse.NotDisengaged => BadRequest("Not Disengaged"),
                DeviceActionResponse.Error => BadRequest("Error"),
                _ => StatusCode(500, "Unknown Error")
            };

        }
    }
}
