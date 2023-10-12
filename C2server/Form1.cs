using System;
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

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool Beep(uint dwFreq, uint dwDuration);




        string data;
        public Form1()
        {
            InitializeComponent();
        }


        //Step2: Press Launch button
        private void launch_btn_Click(object sender, EventArgs e)
        {
            lbloutput.Text = data;
            if (!string.IsNullOrEmpty(data)) {
                Beep(500, 1000);
            }
            
        }

        //Step1: Write some content in cmdbox
        private void cmdbox_TextChanged(object sender, EventArgs e)
        {
            data = cmdbox.Text;
        }

        private void lbloutput_Click(object sender, EventArgs e)
        {

        }

        private void cmdtype_dropdown_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
