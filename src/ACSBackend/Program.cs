
using ACSBackend.Comms.TCP;
using ACSBackend.Database;
using BKDijitalYoklamaBackend.Discord;
using Microsoft.EntityFrameworkCore;
using System;

namespace ACSBackend
{
    public class Program
    {
        public static async Task Main(string[] args)
        {
            Config.Initialize();


            var builder = WebApplication.CreateBuilder(args);

            #region DB
            string conn = Config.Database.ConnectionString;
            builder.Services.AddDbContext<AppDBContext>(options => options.UseNpgsql(conn), ServiceLifetime.Transient, ServiceLifetime.Transient);
            #endregion



            // Add services to the container.

            builder.Services.AddControllers();
            // Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
            builder.Services.AddEndpointsApiExplorer();
            builder.Services.AddSwaggerGen(options => options.IncludeXmlComments(Path.Combine(AppContext.BaseDirectory, "ACSBackend.xml")));

            var app = builder.Build();

            // Configure the HTTP request pipeline.
            if (app.Environment.IsDevelopment())
            {
                app.UseSwagger();
                app.UseSwaggerUI();
            }

            app.UseAuthorization();


            app.MapControllers();

            await DiscordMain.InitDiscord(app.Services);
            await TCPMain.InitTCP(app.Services);

            app.Run();
        }
    }
}
