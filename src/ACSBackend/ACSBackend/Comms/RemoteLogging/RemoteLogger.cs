using System.Net.Sockets;
using System.Net;
using System.Text;

namespace ACSBackend.Comms.RemoteLogging
{
    public class RemoteLogger
    {
        public IPAddress IP { get; private set; }
        public int port { get; private set; }

        public event Action<RemoteLog>? OnLog;

        private UdpClient listener;

        private UdpClient? RedirectClient;

        public RemoteLogger(IPAddress IP, int port)
        {
            this.IP = IP;
            this.port = port;

            listener = new UdpClient(new IPEndPoint(IP, port));

        }

        public async Task ReceiveAsync(CancellationToken ct)
        {
            var data = await listener.ReceiveAsync(ct);
            byte[] buf = data.Buffer;

            if (buf.Length <= 9) throw new Exception("Invalid data length");

            RemoteLog log = new RemoteLog(buf, data.RemoteEndPoint);

            if (OnLog != null) OnLog.Invoke(log);

            if (RedirectClient != null)
            {
                await RedirectClient.SendAsync(buf, buf.Length);
            }

        }


        public void RedirectLogs(IPEndPoint? EP)
        {
            if (RedirectClient != null)
            {
                try
                {
                    RedirectClient.Close();
                }
                finally
                {
                    RedirectClient.Dispose();
                    RedirectClient = null;
                }
            }

            if (EP == null) return;

            RedirectClient = new UdpClient();
            RedirectClient.Connect(EP);
        }
    }

    public class RemoteLog
    {
        public byte[] Original { get; private set; }

        public byte Severity { get; private set; }
        public ulong Timestamp { get; private set; }
        public uint LineNumber { get; private set; }
        public string Message { get; private set; }
        public string FileName { get; private set; }
        public IPEndPoint Sender { get; private set; }

        public RemoteLog(byte[] original, IPEndPoint sender)
        {
            Original = original;
            Sender = sender;

            MemoryStream str = new MemoryStream(original);
            Severity = GetByte(str);
            Timestamp = GetULong(str);
            LineNumber = GetUInt(str);
            Message = GetString(str);
            FileName = GetString(str);
        }


        private static byte GetByte(Stream str)
        {
            return (byte)str.ReadByte();
        }

        private static ulong GetULong(Stream str)
        {
            byte[] buf = new byte[8];
            str.Read(buf, 0, 8);
            return BitConverter.ToUInt64(buf, 0);
        }

        private static uint GetUInt(Stream str)
        {
            byte[] buf = new byte[4];
            str.Read(buf, 0, 4);
            return BitConverter.ToUInt32(buf, 0);
        }

        private static string GetString(Stream str)
        {
            // until null terminatore
            List<byte> buf = new List<byte>();
            while (str.Position < str.Length)
            {
                byte b = (byte)str.ReadByte();
                if (b == 0) break;
                buf.Add(b);
            }

            return Encoding.UTF8.GetString(buf.ToArray());
        }
    }
}
