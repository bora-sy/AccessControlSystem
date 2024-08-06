using ACSBackend.Database;
using ACSBackend.Discord.Extensions;
using ACSBackend.Utils;
using DSharpPlus.Entities;
using DSharpPlus.SlashCommands;

namespace ACSBackend.Discord.Attributes
{
    public class RequirePermissionAttribute : SlashCheckBaseAttribute
    {
        public Permission Permission { get; private set; }

        public RequirePermissionAttribute(Permission perm)
        {
            Permission = perm;
        }

        public override async Task<bool> ExecuteChecksAsync(InteractionContext ctx)
        {
            AppDBContext db = ctx.Services.GetService<AppDBContext>()!;

            var user = db.Users.FirstOrDefault(u => u.DiscordUserID == ctx.User.Id);
            if (user == null)
            {
                await ctx.CreateResponseAsync(DiscordColor.DarkRed, "Error", "You are not registered in the system.");
                return false;
            }

            if (!user.HasPermission(Permission))
            {
                await ctx.CreateResponseAsync(DiscordColor.DarkRed, "Error", "You do not have permission to use this command.");
                return false;
            }

            return true;
        }
    }
}
