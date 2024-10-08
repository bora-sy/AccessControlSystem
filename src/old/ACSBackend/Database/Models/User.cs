﻿using ACSBackend.Utils;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ACSBackend.Database.Models
{
    [Index(nameof(NameSurname))]
    public class User
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int ID { get; set; }

        [Required]
        public string NameSurname { get; set; }

        [Required]
        public Permission Permissions { get; set; }

        public ulong? DiscordUserID { get; set; }


        public ulong? ACSPin { get; set; }
        public byte[]? ACSCardUID { get; set; }


        public bool HasPermission(Permission perm)
        {
            return ACSBackend.Utils.Permissions.HasPermission(Permissions, perm);
        }

    }
}
