using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ACSCommTest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;
        }

        void AppendLog(string log)
        {
            richTextBox1.Text += log + "\n";
        }

        void HandleCommand(CommandID id, byte[] data)
        {
            AppendLog($"Received Command: {id}");

            switch(id)
            {
                case CommandID.PASSWORDENTRY:
                    AppendLog($"Password: {BitConverter.ToInt64(data, 0)}");
                break;

                case CommandID.CARDENTRY:
                    AppendLog($"Card: {data.ToHEXString()}");
                break;

                case CommandID.PLAYTONE:
                    ushort freq = BitConverter.ToUInt16(data, 0);
                    ushort duration = BitConverter.ToUInt16(data, 2);

                    AppendLog($"Freq: {freq} / Duration: {duration}");
                break;
            }
        }


        byte? ReadByte()
        {
            int readRes = serialPort1.ReadByte();
            if (readRes == -1) return null;
            return (byte)readRes;
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            serialPort1.Open();

            Task.Run(async delegate ()
            {
                while (true)
                {
                    byte? len = ReadByte();
                    if (len == null) continue;

                    byte[] data = new byte[len.Value];
                    for (int i = 0; i < len; i++)
                    {
                        byte? b = ReadByte();
                        if (b == null) continue;
                        data[i] = b.Value;
                    }

                    CommandID command = (CommandID)data[0];

                    HandleCommand(command, data.Skip(1).ToArray());
                }
            });
        }


        enum CommandID : byte
        {
            PING = 0,      // Front <--> Core
            PONG,          // Front <--> Core
            RESET,         // Front <--> Core

            FEEDBACK,      // Core   --> Front
            CONFIGMENU,    // Core   --> Front

            PASSWORDENTRY, // Front  --> Core
            CARDENTRY,     // Front  --> Core
            PLAYTONE,      // Front  --> Core

            CONFIG_SETIP,  // Front  --> Core

        };

        void SendData(CommandID commandId, byte[] data)
        {
            byte[] buf = new byte[data.Length + 2];

            buf[0] = (byte)(data.Length + 1);
            buf[1] = (byte)commandId;
            Array.Copy(data, 0, buf, 2, data.Length);

            serialPort1.Write(buf, 0, buf.Length);

            AppendLog($"Sent Command: {commandId}");
            
        }

        private void buttonPing_Click(object sender, EventArgs e)
        {
            SendData(CommandID.PING, new byte[0]);
        }

        private void buttonPong_Click(object sender, EventArgs e)
        {
            SendData(CommandID.PONG, new byte[0]);
        }

        private void buttonConfigMenu_Click(object sender, EventArgs e)
        {
            ulong auth = (ulong)numericUpDownConfigAuth.Value;
            SendData(CommandID.CONFIGMENU, BitConverter.GetBytes(auth));
        }

        private void buttonFeedback_Click(object sender, EventArgs e)
        {
            string text = textBoxFeedback.Text;
            ulong interval = (ulong)numericUpDownFeedbackInterval.Value;

            byte[] data = new byte[text.Length + 8];

            Array.Copy(BitConverter.GetBytes(interval), 0, data, 0, 8);
            Array.Copy(Encoding.ASCII.GetBytes(text), 0, data, 8, text.Length);

            SendData(CommandID.FEEDBACK, data);
        }
    }

}
