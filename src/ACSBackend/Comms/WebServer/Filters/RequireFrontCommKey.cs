using Microsoft.AspNetCore.Mvc.Filters;
using Newtonsoft.Json.Linq;

namespace ACSBackend.Comms.WebServer.Filters
{
    public class RequireFrontCommKey : ActionFilterAttribute
    {
        public override async Task OnActionExecutionAsync(ActionExecutingContext context, ActionExecutionDelegate next)
        {
            if (string.IsNullOrEmpty(DeviceCommMain.FrontCommKey)) return; // TODO: LOG
            if (context.HttpContext.Request.Headers.Authorization == DeviceCommMain.FrontCommKey)
            {
                await next();
                return;
            }

            context.HttpContext.Response.StatusCode = 401;
            await context.HttpContext.Response.WriteAsync("IncorrectCommKey");
        }
    }
}
