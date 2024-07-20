
using Microsoft.EntityFrameworkCore;
using System;

namespace ACSBackend
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            #region DB
            string conn = Config.DB_CNCSTR ?? throw new Exception("DB Connection String is not set!");
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

            app.Run();
        }
    }
}
