using System.Net;
using System.Net.Sockets;
using System.Text;

namespace ACSBackend.Comms.RemoteLogging
{
    public class RemoteLogger
    {
        public IPAddress IP { get; private set; }
        public int port { get; private set; }

        public event Action<RemoteLog> OnLog;

        private UdpClient listener;


        public RemoteLogger(IPAddress IP, int port)
        {
            this.IP = IP;
            this.port = port;

            listener = new UdpClient(new IPEndPoint(IP, port));

            Task.Run(Receiver);
        }

        private async Task Receiver()
        {
            while(true)
            {
                try
               {
                    var data = await listener.ReceiveAsync();
                    byte[] buf = data.Buffer;

                    if(buf.Length <= 9) throw new Exception("Invalid data length");

                    byte severity = buf[0];
                    ulong timestamp = BitConverter.ToUInt64(buf, 1);
                    string message = Encoding.UTF8.GetString(buf, 9, buf.Length - 9);

                    RemoteLog log = new RemoteLog(message, severity, timestamp, data.RemoteEndPoint);

                    if(OnLog != null) OnLog?.Invoke(log);
                }
                catch(Exception ex)
                {
                    Console.WriteLine($"UDP EX: {ex.Message}");
                }
            }
        }
    }

    public class RemoteLog
    {
        public string Message { get; private set; }
        public byte Severity { get; private set; }
        public ulong Timestamp { get; private set; }
        public IPEndPoint Sender { get; private set; }

        public RemoteLog(string message, byte severity, ulong timestamp, IPEndPoint sender)
        {
            Message = message;
            Severity = severity;
            Timestamp = timestamp;
            Sender = sender;
        }
    }
}
