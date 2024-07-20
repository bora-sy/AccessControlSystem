using DSharpPlus.Entities;
using DSharpPlus.SlashCommands;
using Microsoft.Extensions.Logging;

namespace ACSBackend.Discord.Extensions
{
    public static class InteractionContextExtensions
    {
        public static Task<DiscordMessage> EditResponseAsync(this InteractionContext ctx, DiscordMessageBuilder builder)
        {
            return ctx.EditResponseAsync(new DiscordWebhookBuilder(builder));
        }

        public static Task<DiscordMessage> EditResponseAsync(this InteractionContext ctx, string msg)
        {
            return ctx.EditResponseAsync(new DiscordMessageBuilder() { Content = msg });
        }

        public static Task<DiscordMessage> EditResponseAsync(this InteractionContext ctx, params DiscordEmbedBuilder[] embeds)
        {
            return ctx.EditResponseAsync(new DiscordMessageBuilder().AddEmbeds(embeds.Select(x => x.Build())));
        }

        public static Task<DiscordMessage> EditResponseAsync(this InteractionContext ctx, DiscordColor color, string title, string description)
        {
            return ctx.EditResponseAsync(new DiscordEmbedBuilder() { Color = color, Title = title, Description = description });
        }

    }
}
