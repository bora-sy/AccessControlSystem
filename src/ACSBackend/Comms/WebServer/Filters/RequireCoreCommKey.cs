using ACSBackend.Comms.DeviceComm;
using Microsoft.AspNetCore.Mvc.Filters;
using Newtonsoft.Json.Linq;

namespace ACSBackend.Comms.WebServer.Filters
{
    public class RequireCoreCommKey : ActionFilterAttribute
    {
        public override async Task OnActionExecutionAsync(ActionExecutingContext context, ActionExecutionDelegate next)
        {
            if (string.IsNullOrEmpty(DeviceCommMain.CoreCommKey)) return; // TODO: LOG
            if (context.HttpContext.Request.Headers["CommKey"] == DeviceCommMain.CoreCommKey)
            {
                await next();
                return;
            }

            context.HttpContext.Response.StatusCode = 401;
            await context.HttpContext.Response.WriteAsync("IncorrectCommKey");
        }
    }
}
