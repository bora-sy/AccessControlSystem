
using ACSBackend.Comms.DeviceComm;
using ACSBackend.Comms.RemoteLogging;
using ACSBackend.Configurations;
using ACSBackend.Database;
using ACSBackend.Discord;
using ACSBackend.Misc;
using Microsoft.EntityFrameworkCore;

namespace ACSBackend
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            InitConfigurations(builder);
            InitDB(builder);
            InitDeviceComm(builder);
            //InitDiscord(builder);
            InitActionLogger(builder);
            InitRemoteLogging(builder);

            // Add services to the container.

            builder.Services.AddControllers();
            // Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
            builder.Services.AddEndpointsApiExplorer();
            builder.Services.AddSwaggerGen();

            var app = builder.Build();

            // Configure the HTTP request pipeline.
            if (app.Environment.IsDevelopment())
            {
                app.UseSwagger();
                app.UseSwaggerUI();
            }

            app.UseAuthorization();


            app.MapControllers();

            app.Run();
        }

        static void InitConfigurations(WebApplicationBuilder builder)
        {
            var config = builder.Configuration;

            builder.Services.Configure<DBConfiguration>(config.GetSection("DBConfig"));
            builder.Services.Configure<DCConfiguration>(config.GetSection("DCConfig"));
            builder.Services.Configure<DeviceConfiguration>(config.GetSection("DeviceConfig"));
        }

        static void InitDB(WebApplicationBuilder builder)
        {
            string? connectionString = builder.Configuration.GetSection("DBConfig")[nameof(DBConfiguration.ConnectionString)];
            if (connectionString == null) throw new Exception("DB Connection String not found in configuration");

            builder.Services.AddDbContext<AppDBContext>(options => options.UseNpgsql(connectionString));
        }

        static void InitDiscord(WebApplicationBuilder builder)
        {
            //builder.Services.AddSingleton<DiscordService>();
            //builder.Services.AddHostedService<DiscordService>();
        }

        static void InitDeviceComm(WebApplicationBuilder builder)
        {
            builder.Services.AddSingleton<DeviceCommService>();
            builder.Services.AddHostedService<DeviceCommService>();
        }

        static void InitActionLogger(WebApplicationBuilder builder)
        {
            builder.Services.AddScoped<ActionLoggerService>();
        }

        static void InitRemoteLogging(WebApplicationBuilder builder)
        {
            builder.Services.AddHostedService<RemoteLoggingService>();
            //builder.Services.AddSingleton<RemoteLoggingService>();
        }
    }
}
