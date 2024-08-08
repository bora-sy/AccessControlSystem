using ACSBackend.Database;
using ACSBackend.Database.Models;
using ACSBackend.Utils;
using Microsoft.AspNetCore.Mvc.Filters;

namespace ACSBackend.Comms.WebServer.Filters
{

    public class RequirePermission : ActionFilterAttribute
    {
        private readonly Permission _permission;
        public RequirePermission(Permission p)
        {
            _permission = p;
        }

        public override async Task OnActionExecutionAsync(ActionExecutingContext context, ActionExecutionDelegate next)
        {
            var db = context.HttpContext.RequestServices.GetService<AppDBContext>();
            string? pinstr = context.HttpContext.Request.Query["pin"].FirstOrDefault();

            User? user = null;

            if(pinstr == null || !ulong.TryParse(pinstr, out ulong pin) || (user = db.Users.Where(x => x.ACSPin == pin).FirstOrDefault()) == null)
            {
                context.HttpContext.Response.StatusCode = 401;
                await context.HttpContext.Response.WriteAsync("Incorrect PIN");
                return;
            }

            if (!user.HasPermission(_permission))
            {
                context.HttpContext.Response.StatusCode = 401;
                await context.HttpContext.Response.WriteAsync("No Permission");
                return;
            }

            await next();
        }
    }
}
