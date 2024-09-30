using ACSBackend.Discord.Extensions;
using DSharpPlus.SlashCommands;

namespace ACSBackend.Discord.SlashCommands
{
    public class TestCommands(ILogger<TestCommands> logger, IServiceProvider services) : ApplicationCommandModule
    {
        private IServiceScope CreateScope() => _services.CreateScope();
        private T GetRequiredService<T>(IServiceScope scope) where T : class => scope.ServiceProvider.GetRequiredService<T>();

        private readonly ILogger<TestCommands> _logger = logger;
        private readonly IServiceProvider _services = services;


        [SlashCommand("ping", "Replies with pong!")]
        public async Task Ping(InteractionContext ctx)
        {
            await ctx.DeferAsync();

            await ctx.EditResponseAsync("Pong");
        }
    }
}
