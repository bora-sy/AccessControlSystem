using System.Net;
using System.Net.Sockets;
using System.Text;

namespace LogViewer
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            Credentials credentials = GetCredentials();

            await RedirectLogs(credentials);

            NewSession(credentials);
        }

        static void NewSession(Credentials credentials)
        {
        
            UdpClient listener = new UdpClient(credentials.Port);

            Console.WriteLine("Session started");

            while (true)
            {
                try
                {
                    var data = listener.ReceiveAsync().Result;
                    byte[] buf = data.Buffer;

                    if (buf.Length <= 9) throw new Exception("Invalid data length");

                    RemoteLog log = new RemoteLog(buf);

                    Log(log);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"UDP EX: {ex.Message}");
                }
            }
        }


        static async Task RedirectLogs(Credentials creds)
        {
            HttpClient client = new HttpClient();

            string url = $"http://{creds.ACSIP}:5243/logs/redirect?pin={creds.PIN}&ip={creds.IP}&port={creds.Port}";

            var resp = await client.PostAsync(url, null);

            if (!resp.IsSuccessStatusCode)
            {
                Console.WriteLine("Failed to redirect logs");

            }
        }



        static Credentials GetCredentials()
        {
            ulong PIN = GetInput("PIN", (input) => ulong.TryParse(input, out _), (input) => ulong.Parse(input));
            IPAddress ACSIP = GetInput("ACS Server IP Address", (input) => IPAddress.TryParse(input, out _), (input) => IPAddress.Parse(input));
            IPAddress IP = GetInput("Target IP Address", (input) => IPAddress.TryParse(input, out _), (input) => IPAddress.Parse(input));
            int Port = GetInput("Port", (input) => int.TryParse(input, out _), (input) => int.Parse(input));

            return new Credentials(PIN, ACSIP, IP, Port);
        }


        static T GetInput<T>(string title, Func<string?, bool> validate, Func<string, T> convert)
        {
            while(true)
            {
                Console.Write($"{title}: ");
                string? input = Console.ReadLine();
                if (validate(input))
                {
                    return convert(input!);
                }
                else
                {
                    Console.WriteLine("Invalid input");
                }
            }
        }


        static void Log(RemoteLog log)
        {
            char severityChar = ((Severity)log.Severity).ToString()[0];

            string format = $"[{severityChar}][{log.Timestamp}] ({log.FileName}:{log.LineNumber}) {log.Message}";

            Console.ForegroundColor = logColors[(Severity)log.Severity];

            Console.WriteLine(format);

            Console.ForegroundColor = ConsoleColor.White;
        }



        static Dictionary<Severity, ConsoleColor> logColors = new Dictionary<Severity, ConsoleColor>
        {
            { Severity.ERROR, ConsoleColor.Red },
            { Severity.WARNING, ConsoleColor.Yellow },
            { Severity.INFO, ConsoleColor.Blue },
            { Severity.DEBUG, ConsoleColor.Gray },
            { Severity.VERBOSE, ConsoleColor.DarkGray }
        };

        enum Severity : byte
        {
            ERROR = 1,
            WARNING,
            INFO,
            DEBUG,
            VERBOSE
        }

        class Credentials
        {
            public ulong PIN { get; set; }
            public IPAddress IP { get; set; }
            public IPAddress ACSIP { get; set; }
            public int Port { get; set; }

            public Credentials(ulong pin, IPAddress acsip, IPAddress ip, int port)
            {
                PIN = pin;
                IP = ip;
                ACSIP = acsip;
                Port = port;
            }
        }

        class RemoteLog
        {
            public byte[] Original { get; private set; }

            public byte Severity { get; private set; }
            public ulong Timestamp { get; private set; }
            public uint LineNumber { get; private set; }
            public string Message { get; private set; }
            public string FileName { get; private set; }

            public RemoteLog(byte[] original)
            {
                Original = original;

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
}
