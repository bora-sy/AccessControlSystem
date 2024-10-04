using ACSBackend.Utils;
using Microsoft.EntityFrameworkCore;
using System.ComponentModel.DataAnnotations.Schema;
using System.ComponentModel.DataAnnotations;

namespace ACSBackend.Database.Models
{
    public class Config
    {
        [Key]
        public string KEY { get; set; }

        [Required]
        public string VALUE { get; set; }
    }
}
