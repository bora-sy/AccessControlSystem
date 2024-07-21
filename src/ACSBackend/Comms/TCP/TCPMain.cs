using PacketCommunication.Server;

namespace ACSBackend.Comms.TCP
{
    public static class TCPMain
    {
        static PacketServer server = null!;
        
        public static async Task InitTCP(IServiceProvider services)
        {
            server = new PacketServer();
        }
    }
}
