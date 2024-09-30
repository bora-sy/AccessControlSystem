namespace ACSBackend.Configurations
{
    public class DeviceConfiguration
    {
        public string CoreCommKey { get; set; }
        public string FrontCommKey { get; set; }

        public string CoreIP { get; set; }
        public string FrontIP { get; set; }

        public int CoreLoggingPort { get; set; }
        public int FrontLoggingPort { get; set; }
    }
}
