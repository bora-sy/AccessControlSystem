using ACSBackend.Comms.DeviceComm;
using ACSBackend.Database.Models;
using BKDijitalYoklamaBackend.Discord;
using DSharpPlus;
using DSharpPlus.Entities;
using static ACSBackend.Comms.DeviceComm.DeviceCommMain;

namespace ACSBackend.Discord.Integrations
{
    public static class LoggerIntegration
    {
        private static async Task<DiscordChannel >GetLogsChannelAsync()
        {
            var client = DiscordMain.Client;

            ulong chanID = ulong.Parse(ConfigManager.GetConfig(ConfigEnum.DISCORD_LOGS_CHANNEL)!);

            return await client.GetChannelAsync(chanID);
        }

        public static async Task LogAlarmAsync(bool prealarm, bool status)
        {
            var chan = await GetLogsChannelAsync();

            DiscordEmbedBuilder embed;

            string alarmName = prealarm ? "Pre-Alarm" : "Alarm";

            if (!status)
            {
                embed = new DiscordEmbedBuilder()
                {
                    Color = DiscordColor.SpringGreen,
                    Title = $"{alarmName} Aborted"
                };
            }
            else
            {

                embed = new DiscordEmbedBuilder()
                {
                    Color = prealarm ? DiscordColor.Red : DiscordColor.DarkRed,
                    Title = $"!!{alarmName}!!"
                };
            }

            await chan.SendMessageAsync(embed);
        }

        public static async Task LogActionAsync(DeviceAction action, ActionRequestResult result, DeviceActionSource source, User? user)
        {
            var channel = await GetLogsChannelAsync();

            var embed = new DiscordEmbedBuilder()
            {
                Title = "New Action",
                Color = DiscordColor.Aquamarine,
                Description = 
                $"Action: {action}\n" +
                $"Result: {result}\n" +
                $"Source: {source}\n" +
                $"User: {(user == null ? "null" : $"@<{user.DiscordUserID}> - {user.NameSurname}")}"
            };

            await channel.SendMessageAsync(embed);
        }
    }
}
