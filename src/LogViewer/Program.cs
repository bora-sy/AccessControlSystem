using System.Net;
using System.Net.Sockets;
using System.Text;

namespace LogViewer
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Credentials credentials = GetCredentials();

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

                    byte severity = buf[0];
                    ulong timestamp = BitConverter.ToUInt64(buf, 1);
                    string message = Encoding.UTF8.GetString(buf, 9, buf.Length - 9);

                    Log(severity, timestamp, message);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"UDP EX: {ex.Message}");
                }
            }
        }


        static void RedirectLogs(Credentials creds)
        {
            HttpClient client = new HttpClient();
            client.BaseAddress = new Uri($"http://{creds.IP}:{creds.Port}");
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


        static void Log(byte severity, ulong timestamp, string message)
        {
            string format = $"[{((Severity)severity).ToString()[0]}][{timestamp}] {message}";

            Console.ForegroundColor = logColors[(Severity)severity];

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
            ERROR = 0,
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
    }
}
