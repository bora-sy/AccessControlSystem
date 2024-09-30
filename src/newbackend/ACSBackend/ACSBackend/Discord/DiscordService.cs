using ACSBackend.Comms.DeviceComm;
using ACSBackend.Configurations;
using ACSBackend.Database.Models;
using ACSBackend.Database;
using ACSBackend.Discord.SlashCommands;
using DSharpPlus;
using DSharpPlus.Entities;
using DSharpPlus.Interactivity;
using DSharpPlus.Interactivity.Extensions;
using DSharpPlus.SlashCommands;
using Microsoft.Extensions.Options;
using System.Security;
using Microsoft.EntityFrameworkCore;

namespace ACSBackend.Discord
{
    public class DiscordService : BackgroundService
    {
        private readonly ILogger<DiscordService> _logger;
        private readonly DCConfiguration _config;
        private readonly IServiceProvider _services;

        public DiscordClient Client { get; private set; }

        public DiscordService(ILogger<DiscordService> logger, IOptions<DCConfiguration> config, IServiceProvider services)
        {
            _logger = logger;
            _config = config.Value;
            _services = services;

            Client = new DiscordClient(new DiscordConfiguration
            {
                AutoReconnect = true,
                Intents = DiscordIntents.AllUnprivileged,
                MinimumLogLevel = LogLevel.Information,
                Token = _config.Token,
                TokenType = TokenType.Bot
            });


            var interactivity = Client.UseInteractivity(new InteractivityConfiguration
            {
                Timeout = TimeSpan.FromSeconds(30),

            });


            var slash = Client.UseSlashCommands(new SlashCommandsConfiguration
            {
                Services = services
            });

            slash.RegisterCommands<Commands_Misc>();



            slash.SlashCommandErrored += Slash_SlashCommandErrored;
            Client.ComponentInteractionCreated += Client_ComponentInteractionCreated;

        }

        private Task Slash_SlashCommandErrored(SlashCommandsExtension sender, DSharpPlus.SlashCommands.EventArgs.SlashCommandErrorEventArgs args)
        {
            _logger.LogError(args.Exception, "An error occurred while handling a slash command");
            return Task.CompletedTask;
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            await Client.ConnectAsync();
        }

        private async Task Client_ComponentInteractionCreated(DiscordClient sender, DSharpPlus.EventArgs.ComponentInteractionCreateEventArgs args)
        {
            using var scope = _services.CreateScope();
            var _db = scope.ServiceProvider.GetRequiredService<AppDBContext>();
            var _deviceComm = scope.ServiceProvider.GetRequiredService<DeviceCommService>();

            //await args.Interaction.CreateResponseAsync(InteractionResponseType.DeferredMessageUpdate);


            if (!args.Id.StartsWith("action_")) return;

            var action = args.Id.Split("_")[1];

            DeviceAction deviceAction = action switch
            {
                "unlock" => DeviceAction.Unlock,
                "disengage" => DeviceAction.Disengage,
                "engage" => DeviceAction.Engage,
                "abortalarm" => DeviceAction.AbortAlarm,
                _ => (DeviceAction)(-1)
            };

            if (deviceAction == (DeviceAction)(-1)) return;


            User? usr = await _db.Users.Where(x => x.DiscordUserID == args.User.Id).FirstOrDefaultAsync();

            if (usr == null && deviceAction != DeviceAction.AbortAlarm)
            {
                await args.Interaction.CreateResponseAsync(InteractionResponseType.ChannelMessageWithSource, new DiscordInteractionResponseBuilder()
                .AsEphemeral(true)
                .AddEmbed(new DiscordEmbedBuilder()
                {
                    Title = "Permission Error",
                    Description = "You are not registered in the system."
                })
                );
                return;
            }

            var res = await _deviceComm.CoreComm.ExecuteActionAsync(deviceAction, DeviceActionSource.Discord, usr);

            DiscordEmbedBuilder embed = res switch
            {
                ActionRequestResult.SUCCESS => new DiscordEmbedBuilder()
                    .WithColor(DiscordColor.Green)
                    .WithTitle("Success"),

                ActionRequestResult.ERROR => new DiscordEmbedBuilder()
                    .WithColor(DiscordColor.DarkRed)
                    .WithTitle("Unknown Error Occured"),

                ActionRequestResult.ALREADY_ENGAGED => new DiscordEmbedBuilder()
                    .WithColor(DiscordColor.Orange)
                    .WithTitle("Error")
                    .WithDescription("Door is already engaged"),

                ActionRequestResult.ALREADY_DISENGAGED => new DiscordEmbedBuilder()
                    .WithColor(DiscordColor.Orange)
                    .WithTitle("Error")
                    .WithDescription("Door is already disengaged"),

                ActionRequestResult.ALREADY_UNLOCKED => new DiscordEmbedBuilder()
                    .WithColor(DiscordColor.Orange)
                    .WithTitle("Error")
                    .WithDescription("Door is already unlocked"),

                ActionRequestResult.ALARM_NOT_ACTIVE => new DiscordEmbedBuilder()
                    .WithColor(DiscordColor.Orange)
                    .WithTitle("Error")
                    .WithDescription("Alarm is not active"),

                _ => new DiscordEmbedBuilder()
                    .WithColor(DiscordColor.DarkRed)
                    .WithDescription("Unknown Result")
            };

            await args.Interaction.CreateResponseAsync(InteractionResponseType.ChannelMessageWithSource, new DiscordInteractionResponseBuilder().AsEphemeral(true).AddEmbed(embed));
        }
    }
}
