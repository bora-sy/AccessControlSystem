using ACSBackend.Database;
using System.Net.Http.Headers;
using System.Net;
using System.Text.Json.Nodes;
using System.Text;
using ACSBackend.Utils;
using Microsoft.AspNetCore.SignalR;

namespace ACSBackend.Comms.DeviceComm
{
    public static partial class DeviceCommMain
    {

        private static AppDBContext DB = null!;

        public static string CoreCommKey { get; private set; } = null!;
        public static string FrontCommKey { get; private set; } = null!;

        public static async Task InitDeviceComm(IServiceProvider services)
        {
            DB = services.GetService<AppDBContext>()!;

            CoreCommKey = ConfigManager.GetConfig(ConfigEnum.COREDEVICE_COMMKEY)!;
            FrontCommKey = ConfigManager.GetConfig(ConfigEnum.FRONTDEVICE_COMMKEY)!;
        }




        public enum DeviceType
        {
            CORE,
            FRONT
        }

        public enum DeviceAction
        {
            NONE = 0,
            Unlock,
            Engage,
            Disengage,
            AbortAlarm
        }

        public enum DeviceActionSource
        {
            NONE = 0,
            Button,
            Front,
            Discord,
            Web
        }

        public enum ActionRequestResult
        {
            ERROR = -1,
            SUCCESS = 0,
            ALREADY_UNLOCKED,
            ALREADY_ENGAGED,
            ALREADY_DISENGAGED
        }

    }
}
