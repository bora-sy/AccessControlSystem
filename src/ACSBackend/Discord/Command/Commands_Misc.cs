using ACSBackend.Database;
using ACSBackend.Discord.Extensions;
using DSharpPlus.SlashCommands;
using DSharpPlus.Interactivity;
using DSharpPlus.Interactivity.Extensions;
using DSharpPlus.Entities;
using ACSBackend.Discord.DiscordUtils;
using ACSBackend.Comms;
using static ACSBackend.Comms.DeviceCommMain;
using DSharpPlus;
using DSharpPlus.SlashCommands.Attributes;

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

        [SlashCommand("actionsmessage", "Creates an interactive actions mesage")]
        [SlashRequireOwner]
        public async Task ActionsMessage(InteractionContext ctx)
        {
            await ctx.DeferAsync(true);
            try
            {

                var builder = new DiscordMessageBuilder()
                    .AddComponents(
                    new DiscordButtonComponent(ButtonStyle.Success, "action_unlock", "Unlock", false, new DiscordComponentEmoji(DiscordEmoji.FromName(ctx.Client, ":key:"))),
                    new DiscordButtonComponent(ButtonStyle.Primary, "action_disengage", "Disengage", false, new DiscordComponentEmoji(DiscordEmoji.FromName(ctx.Client, ":unlock:"))),
                    new DiscordButtonComponent(ButtonStyle.Danger, "action_engage", "Engage", false, new DiscordComponentEmoji(DiscordEmoji.FromName(ctx.Client, ":lock:")))
                    )
                    .AddEmbed(new DiscordEmbedBuilder()
                    {
                        Title = "Actions",
                        Color = DiscordColor.Orange
                    });

                await ctx.Channel.SendMessageAsync(builder);

                await ctx.EditResponseAsync(DiscordColor.SpringGreen, "Success", "");
            }
            catch(Exception ex)
            {
                await ctx.EditResponseAsync(ex);
            }

        }
    }
}
