using ACSBackend.Comms.DeviceComm;
using ACSBackend.Configurations;
using Microsoft.AspNetCore.Mvc.Filters;
using Microsoft.Extensions.Options;
using Newtonsoft.Json.Linq;

namespace ACSBackend.Comms.WebServer.Controllers.Filters
{
    public class RequireCommKey(DeviceType dt) : ActionFilterAttribute
    {
        DeviceType deviceType = dt;

        public override async Task OnActionExecutionAsync(ActionExecutingContext context, ActionExecutionDelegate next)
        {
            var services = context.HttpContext.RequestServices;

            var _config = services.GetRequiredService<IOptions<DeviceConfiguration>>();

            string commKey = deviceType switch
            {
                DeviceType.CORE => _config.Value.CoreCommKey,
                DeviceType.FRONT => _config.Value.FrontCommKey,
                _ => throw new NotImplementedException()
            };



            if (string.IsNullOrEmpty(commKey)) return; // TODO: LOG

            if (context.HttpContext.Request.Headers["CommKey"] == commKey)
            {
                await next();
                return;
            }

            context.HttpContext.Response.StatusCode = 401;
            await context.HttpContext.Response.WriteAsync("IncorrectCommKey");
        }

    }
}
