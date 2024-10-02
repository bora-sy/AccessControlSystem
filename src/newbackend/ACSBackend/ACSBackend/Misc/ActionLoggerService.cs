using ACSBackend.Configurations;
using ACSBackend.Database;
using ACSBackend.Discord;
using Microsoft.Extensions.Options;

namespace ACSBackend.Misc
{
    public class ActionLoggerService(AppDBContext db, DiscordService dc, IOptions<DCConfiguration> dcConfig)
    {
        AppDBContext _db = db;
        DiscordService _dc = dc;
        DCConfiguration _dcConfig = dcConfig.Value;

    }
}
