using DSharpPlus.Entities;

namespace ACSBackend.Discord.Extensions
{
    public static class MiscExtensions
    {
        public static async Task<byte[]> GetFileContentAsync(this DiscordAttachment att)
        {
            using HttpClient cli = new HttpClient();

            return await cli.GetByteArrayAsync(att.Url);
        }

    }
}
