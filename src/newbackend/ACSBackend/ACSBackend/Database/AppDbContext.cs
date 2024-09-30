using ACSBackend.Database.Models;
using Microsoft.EntityFrameworkCore;

namespace ACSBackend.Database
{
    public class AppDBContext(DbContextOptions<AppDBContext> options) : DbContext(options)
    {
        public DbSet<User> Users { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
        }
    }
}
