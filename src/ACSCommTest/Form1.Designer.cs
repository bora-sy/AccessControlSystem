namespace ACSCommTest
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.buttonConfigMenu = new System.Windows.Forms.Button();
            this.numericUpDownConfigAuth = new System.Windows.Forms.NumericUpDown();
            this.buttonFeedback = new System.Windows.Forms.Button();
            this.textBoxFeedback = new System.Windows.Forms.TextBox();
            this.numericUpDownFeedbackInterval = new System.Windows.Forms.NumericUpDown();
            this.buttonPong = new System.Windows.Forms.Button();
            this.buttonPing = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownConfigAuth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownFeedbackInterval)).BeginInit();
            this.SuspendLayout();
            // 
            // serialPort1
            // 
            this.serialPort1.BaudRate = 921600;
            this.serialPort1.PortName = "COM10";
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(12, 29);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(425, 382);
            this.richTextBox1.TabIndex = 0;
            this.richTextBox1.Text = "";
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(443, 27);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(164, 22);
            this.buttonConnect.TabIndex = 1;
            this.buttonConnect.Text = "Connect";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // buttonConfigMenu
            // 
            this.buttonConfigMenu.Location = new System.Drawing.Point(443, 176);
            this.buttonConfigMenu.Name = "buttonConfigMenu";
            this.buttonConfigMenu.Size = new System.Drawing.Size(164, 35);
            this.buttonConfigMenu.TabIndex = 2;
            this.buttonConfigMenu.Text = "Config Menu";
            this.buttonConfigMenu.UseVisualStyleBackColor = true;
            this.buttonConfigMenu.Click += new System.EventHandler(this.buttonConfigMenu_Click);
            // 
            // numericUpDownConfigAuth
            // 
            this.numericUpDownConfigAuth.Location = new System.Drawing.Point(443, 150);
            this.numericUpDownConfigAuth.Name = "numericUpDownConfigAuth";
            this.numericUpDownConfigAuth.Size = new System.Drawing.Size(164, 20);
            this.numericUpDownConfigAuth.TabIndex = 3;
            // 
            // buttonFeedback
            // 
            this.buttonFeedback.Location = new System.Drawing.Point(443, 339);
            this.buttonFeedback.Name = "buttonFeedback";
            this.buttonFeedback.Size = new System.Drawing.Size(164, 35);
            this.buttonFeedback.TabIndex = 4;
            this.buttonFeedback.Text = "Feedback";
            this.buttonFeedback.UseVisualStyleBackColor = true;
            this.buttonFeedback.Click += new System.EventHandler(this.buttonFeedback_Click);
            // 
            // textBoxFeedback
            // 
            this.textBoxFeedback.Location = new System.Drawing.Point(443, 248);
            this.textBoxFeedback.Multiline = true;
            this.textBoxFeedback.Name = "textBoxFeedback";
            this.textBoxFeedback.Size = new System.Drawing.Size(164, 59);
            this.textBoxFeedback.TabIndex = 5;
            // 
            // numericUpDownFeedbackInterval
            // 
            this.numericUpDownFeedbackInterval.Location = new System.Drawing.Point(443, 313);
            this.numericUpDownFeedbackInterval.Name = "numericUpDownFeedbackInterval";
            this.numericUpDownFeedbackInterval.Size = new System.Drawing.Size(164, 20);
            this.numericUpDownFeedbackInterval.TabIndex = 6;
            // 
            // buttonPong
            // 
            this.buttonPong.Location = new System.Drawing.Point(443, 97);
            this.buttonPong.Name = "buttonPong";
            this.buttonPong.Size = new System.Drawing.Size(164, 29);
            this.buttonPong.TabIndex = 7;
            this.buttonPong.Text = "Pong";
            this.buttonPong.UseVisualStyleBackColor = true;
            this.buttonPong.Click += new System.EventHandler(this.buttonPong_Click);
            // 
            // buttonPing
            // 
            this.buttonPing.Location = new System.Drawing.Point(443, 62);
            this.buttonPing.Name = "buttonPing";
            this.buttonPing.Size = new System.Drawing.Size(164, 29);
            this.buttonPing.TabIndex = 8;
            this.buttonPing.Text = "Ping";
            this.buttonPing.UseVisualStyleBackColor = true;
            this.buttonPing.Click += new System.EventHandler(this.buttonPing_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(619, 450);
            this.Controls.Add(this.buttonPing);
            this.Controls.Add(this.buttonPong);
            this.Controls.Add(this.numericUpDownFeedbackInterval);
            this.Controls.Add(this.textBoxFeedback);
            this.Controls.Add(this.buttonFeedback);
            this.Controls.Add(this.numericUpDownConfigAuth);
            this.Controls.Add(this.buttonConfigMenu);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.richTextBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownConfigAuth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownFeedbackInterval)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Button buttonConfigMenu;
        private System.Windows.Forms.NumericUpDown numericUpDownConfigAuth;
        private System.Windows.Forms.Button buttonFeedback;
        private System.Windows.Forms.TextBox textBoxFeedback;
        private System.Windows.Forms.NumericUpDown numericUpDownFeedbackInterval;
        private System.Windows.Forms.Button buttonPong;
        private System.Windows.Forms.Button buttonPing;
    }
}

