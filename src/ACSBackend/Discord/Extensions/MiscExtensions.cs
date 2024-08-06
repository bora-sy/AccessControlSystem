using DSharpPlus.Entities;
using System.Runtime.CompilerServices;
using ACSBackend.Comms;
using static ACSBackend.Comms.DeviceCommMain;

namespace ACSBackend.Discord.Extensions
{
    public static class MiscExtensions
    {
        public static async Task<byte[]?> GetFileContentAsync(this DiscordAttachment att)
        {
            try
            {
            using HttpClient cli = new HttpClient();

            return await cli.GetByteArrayAsync(att.Url);
            }
            catch (Exception)
            {
                return null;
            }
        }

        public static DiscordEmbedBuilder ToDiscordEmbed(this ActionRequestResult res)
        {
            switch (res)
            {
                case ActionRequestResult.SUCCESS:
                    return new DiscordEmbedBuilder()
                        .WithColor(DiscordColor.Green)
                        .WithTitle("Success");

                case ActionRequestResult.ERROR:
                    return new DiscordEmbedBuilder()
                        .WithColor(DiscordColor.DarkRed)
                        .WithTitle("Unknown Error Occured");

                case ActionRequestResult.ALREADY_ENGAGED:
                    return new DiscordEmbedBuilder()
                        .WithColor(DiscordColor.Orange)
                        .WithTitle("Error")
                        .WithDescription("Door is already engaged");

                case ActionRequestResult.ALREADY_DISENGAGED:
                    return new DiscordEmbedBuilder()
                        .WithColor(DiscordColor.Orange)
                        .WithTitle("Error")
                        .WithDescription("Door is already disengaged");

                case ActionRequestResult.ALREADY_UNLOCKED:
                    return new DiscordEmbedBuilder()
                        .WithColor(DiscordColor.Orange)
                        .WithTitle("Error")
                        .WithDescription("Door is already unlocked");

                default:
                    return new DiscordEmbedBuilder()
                        .WithColor(DiscordColor.DarkRed)
                        .WithDescription("Unknown Result");
            }
        }
    }
}
