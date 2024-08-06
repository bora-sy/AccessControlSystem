using ACSBackend.Discord.Commands;
using DSharpPlus.SlashCommands;
using DSharpPlus;
using System.Reflection;
using ACSBackend;
using DSharpPlus.Interactivity.Extensions;

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

            slash.RegisterCommands<Commands_Misc>();
            slash.RegisterCommands<Commands_ACSBasic>();

            await Client.ConnectAsync();
        }

    }
}
