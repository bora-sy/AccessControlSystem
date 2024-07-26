using Microsoft.AspNetCore.Mvc.Filters;
using Newtonsoft.Json.Linq;

namespace ACSBackend.Comms.WebServer.Filters
{
    public class RequireDeviceKey : ActionFilterAttribute
    {
        public override async Task OnActionExecutionAsync(ActionExecutingContext context, ActionExecutionDelegate next)
        {
            if (string.IsNullOrEmpty(DeviceCommMain.DeviceAPIKey)) return; // TODO: LOG
            if (context.HttpContext.Request.Headers.Authorization == DeviceCommMain.DeviceAPIKey)
            {
                await next();
                return;
            }

            context.HttpContext.Response.StatusCode = 401;
            await context.HttpContext.Response.WriteAsync("IncorrectDeviceKey");
        }
    }
}
