using DSharpPlus.Entities;
using System.Runtime.CompilerServices;
using static ACSBackend.Comms.DeviceComm.DeviceCommMain;

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

    }
}
