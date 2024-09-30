using ACSBackend.Discord.Extensions;
using DSharpPlus.Entities;
using DSharpPlus;
using DSharpPlus.SlashCommands;
using DSharpPlus.SlashCommands.Attributes;
using System.Net;
using ACSBackend.Comms.DeviceComm;

namespace ACSBackend.Discord.SlashCommands
{
    public class Commands_Misc(ILogger<Commands_Misc> logger, IServiceProvider services, DeviceCommService deviceComm) : ApplicationCommandModule
    {
        private IServiceScope CreateScope() => _services.CreateScope();
        private T GetRequiredService<T>(IServiceScope scope) where T : class => scope.ServiceProvider.GetRequiredService<T>();

        private readonly ILogger<Commands_Misc> _logger = logger;
        private readonly IServiceProvider _services = services;

        private readonly DeviceCommService _deviceComm = deviceComm;

        [SlashCommand("ping", "Replies with pong")]
        public async Task Ping(InteractionContext ctx)
        {
            await ctx.DeferAsync();

            await ctx.EditResponseAsync("Pong!");
        }


        [SlashRequireOwner]
        [SlashCommand("actionsmessage", "Creates an interactive actions mesage")]
        public async Task ActionsMessage(InteractionContext ctx)
        {
            await ctx.DeferAsync(true);
            try
            {
                var builder = new DiscordMessageBuilder()
                    .AddComponents(
                    new DiscordButtonComponent(ButtonStyle.Success, "action_unlock", "Unlock", false, new DiscordComponentEmoji(DiscordEmoji.FromName(ctx.Client, ":key:"))),
                    new DiscordButtonComponent(ButtonStyle.Danger, "action_abortalarm", "Abort Alarm", false, new DiscordComponentEmoji(DiscordEmoji.FromName(ctx.Client, ":rotating_light:")))
                    )
                    .AddComponents(
                    new DiscordButtonComponent(ButtonStyle.Primary, "action_disengage", "Disengage", false, new DiscordComponentEmoji(DiscordEmoji.FromName(ctx.Client, ":unlock:"))),
                    new DiscordButtonComponent(ButtonStyle.Secondary, "action_engage", "Engage", false, new DiscordComponentEmoji(DiscordEmoji.FromName(ctx.Client, ":lock:")))
                    )
                    .AddEmbed(new DiscordEmbedBuilder()
                    {
                        Title = "Actions",
                        Color = DiscordColor.Orange
                    });

                await ctx.Channel.SendMessageAsync(builder);

                await ctx.EditResponseAsync(DiscordColor.SpringGreen, "Success", "");
            }
            catch (Exception ex)
            {
                await ctx.EditResponseAsync(ex);
            }

        }

        [SlashCommand("pingcore", "Pings the Core Device")]
        public async Task PingCore(InteractionContext ctx)
        {
            await ctx.DeferAsync();

            bool suc = await _deviceComm.CoreComm.PingDevice();

            if (suc) await ctx.EditResponseAsync(DiscordColor.SpringGreen, "Success", "Successfully pinged the Core Device");
            else await ctx.EditResponseAsync(DiscordColor.Red, "Error", "Something went wrong while pinging the Core Device");
        }
    }
}
