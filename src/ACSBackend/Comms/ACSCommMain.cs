using ACSBackend.Database;

namespace ACSBackend.Comms
{
    public static class ACSCommMain
    {
        private static AppDBContext DB;
        public static async Task InitACSComm(IServiceProvider services)
        {
            DB = services.GetService<AppDBContext>();
        }
    }
}
