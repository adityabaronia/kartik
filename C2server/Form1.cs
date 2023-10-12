﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace C2server
{
    public partial class Form1 : Form
    {
        string data;

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool Beep(uint dwFreq, uint dwDuration);

        public Form1()
        {
            InitializeComponent();
        }

        //Step1: Write some content in cmdbox
        private void cmdbox_TextChanged(object sender, EventArgs e)
        {
            data = cmdbox.Text;
        }

        //Step2: Press Launch button
        private void launch_btn_Click(object sender, EventArgs e)
        {
            outbox.Text = data;
            
            if (!string.IsNullOrEmpty(data)) {
                Beep(500, 1000);
            }
            
        }

        private void cmdtype_dropdown_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
        }

        private void startc2_Click(object sender, EventArgs e)
        {
            C2server.serverModule sModule = new C2server.serverModule();
            sModule.Initialize_Internet((message) =>
            {
                // Use Invoke to update the UI safely from a different thread
                this.Invoke((MethodInvoker)delegate
                {
                    outbox.Text = message;
                });
            });
        }

        private void outbox_TextChanged(object sender, EventArgs e)
        {
            
        }
    }


    public class serverModule
    {
        const int INTERNET_OPEN_TYPE_PRECONFIG = 0; // use registry configuration
        const int INTERNET_OPEN_TYPE_DIRECT = 1; // direct to net
        const int INTERNET_OPEN_TYPE_PROXY = 3; // via named proxy
        const int INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY = 4; // prevent using java/script/INS

        [DllImport("wininet.dll", SetLastError = true, CharSet = CharSet.Auto)]
        static extern IntPtr InternetOpen(string lpszAgent, int dwAccessType, string lpszProxyName, string lpszProxyBypass, int dwFlags);

        public int Initialize_Internet(Action<string> updateTextBox)
        {
            IntPtr InterHandle = InternetOpen("browser", INTERNET_OPEN_TYPE_DIRECT, null, null, 0);
            if (IntPtr.Zero == InterHandle)
            {
                updateTextBox("Error in InternetOpen API");
                return 1;
            }
            updateTextBox("InternetOpen API working");

            return 0;
        }

    }




}
