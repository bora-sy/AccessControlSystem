using ACSBackend.Configurations;
using ACSBackend.Discord.SlashCommands;
using DSharpPlus;
using DSharpPlus.Interactivity;
using DSharpPlus.Interactivity.Extensions;
using DSharpPlus.SlashCommands;
using Microsoft.Extensions.Options;

namespace ACSBackend.Discord
{
    public class DiscordService : IHostedService
    {
        private readonly ILogger<DiscordService> _logger;
        private readonly DCConfiguration _config;

        public DiscordClient Client { get; private set; }

        public DiscordService(ILogger<DiscordService> logger, IOptions<DCConfiguration> config, IServiceProvider services)
        {
            _logger = logger;
            _config = config.Value;

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

            slash.RegisterCommands<TestCommands>();



            slash.SlashCommandErrored += Slash_SlashCommandErrored;

        }

        private Task Slash_SlashCommandErrored(SlashCommandsExtension sender, DSharpPlus.SlashCommands.EventArgs.SlashCommandErrorEventArgs args)
        {
            _logger.LogError(args.Exception, "An error occurred while handling a slash command");
            return Task.CompletedTask;
        }

        public async Task StartAsync(CancellationToken cancellationToken)
        {
            await Client.ConnectAsync();
        }

        public async Task StopAsync(CancellationToken cancellationToken)
        {

        }
    }
}
