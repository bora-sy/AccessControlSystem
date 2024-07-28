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

        void HandleCommand(CommandID id, byte[] data)
        {
            richTextBox1.Text += $"Received: {id}\n";

            richTextBox1.Text +=  BitConverter.ToInt64(data, 0).ToString() + "\n";
        }


        byte? ReadByte()
        {
            int readRes = serialPort1.ReadByte();
            if (readRes == -1) return null;
            return (byte)readRes;
        }

        private void button1_Click(object sender, EventArgs e)
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
    }

}
