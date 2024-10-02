using System.ComponentModel.DataAnnotations.Schema;
using System.ComponentModel.DataAnnotations;
using ACSBackend.Comms.DeviceComm;
using Microsoft.EntityFrameworkCore;

namespace ACSBackend.Database.Models
{
    public class ActionLog
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int ID { get; set; }

        public int? UserId { get; set; } //FOREIGN KEY

        [ForeignKey(nameof(UserId))]
        [DeleteBehavior(DeleteBehavior.SetNull)]
        public User? User { get; set; } //FOREIGN KEY

        [Required]
        public DateTime Time { get; set; }

        [Required]
        public DeviceActionSource ActionSource { get; set; }

        [Required]
        public DeviceAction Action{ get; set; }

        [Required]
        public ActionRequestResult Result { get; set; }

    }
}
