using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ACSBackend.Database.Models
{
    public class User
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int ID { get; set; }

        [Required]
        public string NameSurname { get; set; } = null!;

        public ulong? DiscordUserID { get; set; }

        public ulong? ACSPin { get; set; }
        public uint? ACSCardUID { get; set; }
    }
}
