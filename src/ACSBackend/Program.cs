
using ACSBackend.Comms;
using ACSBackend.Comms.RemoteLogging;
using ACSBackend.Database;
using BKDijitalYoklamaBackend.Discord;
using Microsoft.EntityFrameworkCore;
using Microsoft.OpenApi.Models;
using System;
using System.Net;

namespace ACSBackend
{
    public class Program
    {
        public static async Task Main(string[] args)
        {

            try
            {


                var builder = WebApplication.CreateBuilder(args);

                #region DB
                string conn = ConfigManager.DBCNCSTR;
                builder.Services.AddDbContext<AppDBContext>(options => options.UseNpgsql(conn), ServiceLifetime.Transient, ServiceLifetime.Transient);
                #endregion

                builder.Services.AddControllers();

                builder.Services.AddEndpointsApiExplorer();
                builder.Services.AddSwaggerGen(options =>
                {
                    options.IncludeXmlComments(Path.Combine(AppContext.BaseDirectory, "ACSBackend.xml"));
                    options.AddSecurityDefinition("Bearer", new OpenApiSecurityScheme
                    {
                        In = ParameterLocation.Header,
                        Description = "Please enter Device API Key",
                        Name = "Authorization",
                        Type = SecuritySchemeType.ApiKey,
                        BearerFormat = "API Key",
                        Scheme = "Bearer",
                    });

                    options.AddSecurityRequirement(new OpenApiSecurityRequirement
                    {
                        {
                          new OpenApiSecurityScheme
                          {
                              Reference = new OpenApiReference
                              {
                                  Type = ReferenceType.SecurityScheme,
                                  Id = "Bearer"
                              }
                          },
                          new string[] {}
                        }
                    });

                });

                var app = builder.Build();

                ConfigManager.InitConfigManager(app.Services);


                // Configure the HTTP request pipeline.
                if (app.Environment.IsDevelopment())
                {
                    app.UseSwagger();
                    app.UseSwaggerUI();
                }

                app.UseAuthorization();


                app.MapControllers();

                await DiscordMain.InitDiscord(app.Services);
                await DeviceCommMain.InitDeviceComm(app.Services);
                await RemoteLoggingMain.Initialize();

                app.Run();
            }
            catch (Exception ex)
            {

            }
        }
    }
}
