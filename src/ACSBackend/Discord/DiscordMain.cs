using ACSBackend.Discord.Commands;
using DSharpPlus.SlashCommands;
using DSharpPlus;
using System.Reflection;
using ACSBackend;
using DSharpPlus.Interactivity.Extensions;
using static ACSBackend.Comms.DeviceComm.DeviceCommMain;
using ACSBackend.Discord.Extensions;
using DSharpPlus.Entities;

namespace BKDijitalYoklamaBackend.Discord
{

    public static class DiscordMain
    {
        public static DiscordClient Client { get; private set; } = null!;
        
        public static async Task InitDiscord(IServiceProvider services)
        {

            Client = new DiscordClient(new DiscordConfiguration()
            {
                Intents = DiscordIntents.All,
                TokenType = TokenType.Bot,
                Token = ConfigManager.GetConfig(ConfigEnum.DISCORD_TOKEN)
            });

            var slash = Client.UseSlashCommands(new SlashCommandsConfiguration()
            {
                Services = services
            });

            Client.UseInteractivity(new DSharpPlus.Interactivity.InteractivityConfiguration()
            {
                
            });

            Client.ComponentInteractionCreated += Client_ComponentInteractionCreated;

            slash.RegisterCommands<Commands_Misc>();

            await Client.ConnectAsync();
        }

        private static async Task Client_ComponentInteractionCreated(DiscordClient sender, DSharpPlus.EventArgs.ComponentInteractionCreateEventArgs args)
        {
            if (!args.Id.StartsWith("action_")) return;

            var action = args.Id.Split("_")[1];

            DeviceAction deviceAction = action switch
            {
                "unlock" => DeviceAction.Unlock,
                "disengage" => DeviceAction.Disengage,
                "engage" => DeviceAction.Engage,
                "abortalarm" => DeviceAction.AbortAlarm
                _ => (DeviceAction)(-1)
            };

            if (deviceAction == (DeviceAction)(-1)) return;

            var res = await Core.ExecuteAction(deviceAction, DeviceActionSource.Discord);

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

                _ => new DiscordEmbedBuilder()
                    .WithColor(DiscordColor.DarkRed)
                    .WithDescription("Unknown Result")
            };

            await args.Interaction.CreateResponseAsync(InteractionResponseType.ChannelMessageWithSource, new DiscordInteractionResponseBuilder().AsEphemeral(true).AddEmbed(embed));
        }
    }
}
