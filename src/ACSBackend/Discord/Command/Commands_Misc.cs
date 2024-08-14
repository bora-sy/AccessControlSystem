using ACSBackend.Database;
using ACSBackend.Discord.Extensions;
using DSharpPlus.SlashCommands;
using DSharpPlus.Interactivity;
using DSharpPlus.Interactivity.Extensions;
using DSharpPlus.Entities;
using ACSBackend.Discord.DiscordUtils;
using static ACSBackend.Comms.DeviceComm.DeviceCommMain;
using DSharpPlus;
using DSharpPlus.SlashCommands.Attributes;
using ACSBackend.Comms.WebServer.Controllers;
using ACSBackend.Comms.DeviceComm;
using System.Net;

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
            catch(Exception ex)
            {
                await ctx.EditResponseAsync(ex);
            }

        }

        [SlashCommand("pingcore", "Pings the Core Device")]
        public async Task PingCore(InteractionContext ctx)
        {
            await ctx.DeferAsync();

            bool suc = await DeviceCommMain.Core.PingDevice();

            if (suc) await ctx.EditResponseAsync(DiscordColor.SpringGreen, "Success", "Successfully pinged the Core Device");
            else await ctx.EditResponseAsync(DiscordColor.Red, "Error", "Something went wrong while pinging the Core Device");
        }

        [SlashCommand("setcoreip", "Set Core Device IP Address")]
        public async Task SetCoreIP(InteractionContext ctx, [Option("ip_addr", "IP Address")] string ipStr)
        {
            await ctx.DeferAsync();

            if(!IPAddress.TryParse(ipStr, out _))
            {
                await ctx.EditResponseAsync(DiscordColor.Red, "Error", "Invalid IP Address");
                return;
            }

            ConfigManager.SetConfig(ConfigEnum.COREDEVICE_IP, ipStr);

            bool suc = await DeviceCommMain.Core.PingDevice();

            if (suc) await ctx.EditResponseAsync(DiscordColor.SpringGreen, "Success", "Successfully changed the IP Address & pinged the Core Device");
            else await ctx.EditResponseAsync(DiscordColor.Red, "Error", "Something went wrong while updating IP Address");
        }

    }
}
