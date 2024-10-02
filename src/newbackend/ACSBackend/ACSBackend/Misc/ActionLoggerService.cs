using ACSBackend.Comms.DeviceComm;
using ACSBackend.Configurations;
using ACSBackend.Database;
using ACSBackend.Database.Models;
using ACSBackend.Discord;
using DSharpPlus;
using DSharpPlus.Entities;
using Microsoft.Extensions.Options;

namespace ACSBackend.Misc
{
    public class ActionLoggerService(AppDBContext db, DiscordService dc, IOptions<DCConfiguration> dcConfig, ILogger<ActionLoggerService> logger)
    {
        AppDBContext _db = db;
        DiscordClient _dc = dc.Client;
        DCConfiguration _dcConfig = dcConfig.Value;
        ILogger<ActionLoggerService> _logger = logger;

        public async Task LogActionAsync(DeviceActionSource source, DeviceAction action, ActionRequestResult result, User? user = null)
        {
            var log = new ActionLog
            {
                User = user,
                Time = DateTime.UtcNow,
                ActionSource = source,
                Action = action,
                Result = result
            };

            await Task.WhenAll(LogToDB(log), LogToDiscord(log));
        }

        private async Task LogToDB(ActionLog log)
        {
            await _db.ActionLogs.AddAsync(log);
            await _db.SaveChangesAsync();
        }

        private async Task LogToDiscord(ActionLog log)
        {
            var channel = await _dc.GetChannelAsync(_dcConfig.ActionLogChannel);

            var embed = new DiscordEmbedBuilder()
                .WithColor(DiscordColor.Gold)
                .WithTimestamp(log.Time.ToLocalTime())
                .WithAuthor(log.User == null ? "Unknown" : $"{(log.User.DiscordUserID != null ? $"<@{log.User.DiscordUserID}> - " : "")}{log.User.NameSurname}", log.User?.DiscordUserID == null ? null : (await channel.Guild.GetMemberAsync(log.User.DiscordUserID.Value)).AvatarUrl)
                .AddField("Action", log.Action.ToString())
                .AddField("Result", log.Result.ToString());

            await channel.SendMessageAsync(embed);
        }
    }
}
