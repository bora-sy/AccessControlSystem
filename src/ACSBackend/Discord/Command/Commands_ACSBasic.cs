using ACSBackend.Database;
using ACSBackend.Discord.Extensions;
using DSharpPlus.SlashCommands;
using DSharpPlus.Interactivity;
using DSharpPlus.Interactivity.Extensions;
using DSharpPlus.Entities;
using ACSBackend.Discord.DiscordUtils;
using ACSBackend.Comms;
using static ACSBackend.Comms.DeviceCommMain;
using ACSBackend.Discord.Attributes;

namespace ACSBackend.Discord.Commands
{
    public class Commands_ACSBasic(
        ILogger<Commands_ACSBasic> logger,
        IConfiguration configuration,
        AppDBContext appDbContext) : ApplicationCommandModule
    {
        private readonly IConfiguration _configuration = configuration;
        private readonly ILogger<Commands_ACSBasic> _logger = logger;
        private readonly AppDBContext dbContext = appDbContext;


        [SlashCommand("unlock", "Unlock the door")]
        [RequirePermission(Utils.Permission.USE_ACTIONS | Utils.Permission.ACTION_UNLOCK)]
        public async Task Unlock(InteractionContext ctx)
        {
            await ctx.DeferAsync();

            var resp = await DeviceCommMain.Core.ExecuteAction(DeviceAction.Unlock);

            await ctx.EditResponseAsync(resp.ToDiscordEmbed());
        }


        [SlashCommand("disengage", "Disengage the door")]
        [RequirePermission(Utils.Permission.USE_ACTIONS | Utils.Permission.ACTION_DISENGAGE)]
        public async Task Disengage(InteractionContext ctx)
        {
            await ctx.DeferAsync();

            var resp = await DeviceCommMain.Core.ExecuteAction(DeviceAction.Disengage);

            await ctx.EditResponseAsync(resp.ToDiscordEmbed());
        }

        [SlashCommand("engage", "Engage the door")]
        [RequirePermission(Utils.Permission.USE_ACTIONS | Utils.Permission.ACTION_DISENGAGE)]
        public async Task Engage(InteractionContext ctx)
        {
            await ctx.DeferAsync();

            var resp = await DeviceCommMain.Core.ExecuteAction(DeviceAction.Engage);

            await ctx.EditResponseAsync(resp.ToDiscordEmbed());
        }

    }
}
