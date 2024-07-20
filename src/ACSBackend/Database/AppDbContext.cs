using ACSBackend.Database.Models;
using Microsoft.EntityFrameworkCore;
using System.Data;

namespace ACSBackend.Database
{
    public class AppDBContext(DbContextOptions<AppDBContext> options) : DbContext(options)
    {
        public DbSet<User> Users { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder) //Define relationships and indexes here
        {

        }
    }
}
