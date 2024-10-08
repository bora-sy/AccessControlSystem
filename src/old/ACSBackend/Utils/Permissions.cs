﻿namespace ACSBackend.Utils
{
    [Flags]
    public enum Permission : ulong
    {
        NONE = 0,

        USE_ACTIONS       = 1 << 0,
        ACTION_UNLOCK = 1 << 1,
        ACTION_DISENGAGE  = 1 << 2,

        MANAGE_LOGS       = 1 << 49,
        MANAGE_PERMS      = 1 << 50,



        ALL = ulong.MaxValue
    }


    public static class Permissions
    {
        public static bool HasPermission(this Permission perms, Permission perm)
        {
            return (perms & perm) == perm;
        }

        public static void RemovePermissions(ref this Permission main, params Permission[] perms)
        {
            foreach (var p in perms)
            {
                main &= ~p;
            }
        }

        public static void AddPermissions(ref this Permission main, params Permission[] perms)
        {
            foreach (var p in perms)
            {
                main |= p;
            }
        }
    }
}
