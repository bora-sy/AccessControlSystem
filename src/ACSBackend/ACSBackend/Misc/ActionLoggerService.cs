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
    public class ActionLoggerService(AppDBContext db,/* DiscordService dc, */ IOptions<DCConfiguration> dcConfig, ILogger<ActionLoggerService> logger)
    {
        AppDBContext _db = db;
        //DiscordClient _dc = dc.Client;
        DCConfiguration _dcConfig = dcConfig.Value;
        ILogger<ActionLoggerService> _logger = logger;

        public async Task LogActionAsync(DeviceActionSource source, DeviceAction action, ActionRequestResult result, User? user = null)
        {
            var log = new ActionLog
            {
                UserId = user?.ID,
                Time = DateTime.UtcNow,
                ActionSource = source,
                Action = action,
                Result = result
            };


            await LogToDB(log);
            //await LogToDiscord(log);
            
        }

        private async Task LogToDB(ActionLog log)
        {
            await _db.ActionLogs.AddAsync(log);
            await _db.SaveChangesAsync();
        }


        /*
        private async Task LogToDiscord(ActionLog log)
        {
            var channel = await _dc.GetChannelAsync(_dcConfig.ActionLogChannel);
            var guild = await _dc.GetGuildAsync(channel.GuildId!.Value);

            var embed = new DiscordEmbedBuilder();
            embed.Color = DiscordColor.Gold;
            embed.Timestamp = log.Time.ToLocalTime();
            embed.Author = new DiscordEmbedBuilder.EmbedAuthor();
            embed.Author.Name = log.User == null ? "Unknown" : $"{(log.User.DiscordUserID != null ? $"<@{log.User.DiscordUserID}> - " : "")}{log.User.NameSurname}";

            DiscordMember? member = log.User?.DiscordUserID == null ? null : await guild.GetMemberAsync(log.User.DiscordUserID.Value);

            embed.Author.IconUrl = member == null ? null : member.AvatarUrl;
            embed = embed
            .AddField("Action", log.Action.ToString())
            .AddField("Result", log.Result.ToString());

            await channel.SendMessageAsync(embed);
        }
        */
    }
}
