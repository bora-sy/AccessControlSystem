using ACSBackend.Database;
using ACSBackend.Discord.Extensions;
using ACSBackend.Web.Controllers;
using DSharpPlus.SlashCommands;
using DSharpPlus.Interactivity;
using DSharpPlus.Interactivity.Extensions;
using DSharpPlus.Entities;
using ACSBackend.Discord.DiscordUtils;

namespace ACSBackend.Discord.Commands
{
    public class Commands_Misc(
        ILogger<Commands_Misc> logger,
        IConfiguration configuration,
        AppDBContext appDbContext) : ApplicationCommandModule
    {
        private readonly IConfiguration _configuration = configuration;
        private readonly ILogger<Commands_Misc> _logger = logger;
        private readonly AppDBContext dbContext = appDbContext;


        [SlashCommand("ping", "Replies with pong")]
        public async Task Ping(InteractionContext ctx)
        {
            await ctx.DeferAsync();


            await ctx.EditResponseAsync("Pong!");
        }

    }
}
