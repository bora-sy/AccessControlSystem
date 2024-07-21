using PacketCommunication;
using PacketCommunication.Server;
using System.Net;
using System.Reflection;

namespace ACSBackend.Comms.TCP
{

    public class TCPService
    {
        PacketServer server = null!;

        private ILogger<TCPService> _logger;
        
        public TCPService(ILogger<TCPService> logger)
        {
            _logger = logger;

            var packets = GetPackets("ACSBackend.Comms.TCP.TCPPackets", out int length);
            server = new PacketServer(IPEndPoint.Parse(Config.Comm.TCP_Endpoint), packets);

            server.StartListening();

            server.ServerErrored += Server_ServerErrored;
            server.ClientConnected += Server_ClientConnected;
            server.ClientDisconnected += Server_ClientDisconnected;
        }

        private void Server_ClientDisconnected(object? sender, Client e)
        {

        }

        private void Server_ClientConnected(object? sender, Client e)
        {

        }

        private void Server_ServerErrored(object? sender, Exception e)
        {

        }

        private PacketCollection GetPackets(string @namespace, out int length)
        {
            Assembly asm = Assembly.GetExecutingAssembly();

            Type[] packetTypes = asm.GetTypes()
                .Where(
                    x =>
                        x.IsValueType
                        && x.Namespace.StartsWith(@namespace)
                        && x.IsAssignableTo(typeof(IBasePacket))
                ).ToArray();

            length = packetTypes.Length;

            PacketCollection packets = new PacketCollection(packetTypes);

            return packets;
        }
    }
}
