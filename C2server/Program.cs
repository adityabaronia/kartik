using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace C2server
{
    internal static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
            
        }
    }


    
}

namespace server {
    public class serverModule
    {
        const int INTERNET_OPEN_TYPE_PRECONFIG = 0; // use registry configuration
        const int INTERNET_OPEN_TYPE_DIRECT = 1; // direct to net
        const int INTERNET_OPEN_TYPE_PROXY = 3; // via named proxy
        const int INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY = 4; // prevent using java/script/INS

        [DllImport("wininet.dll", SetLastError = true, CharSet = CharSet.Auto)]
        static extern IntPtr InternetOpen(string lpszAgent, int dwAccessType, string lpszProxyName, string lpszProxyBypass, int dwFlags);



        private int Initialize_Internet()
        {
            C2server.Form1 frm = new C2server.Form1();
            IntPtr InterHandle = InternetOpen("browser", INTERNET_OPEN_TYPE_DIRECT, null, null, 0);
            if (IntPtr.Zero == InterHandle)
            {
                
                frm.lbloutput.Text = "Error";
                //Form1.lbloutput_Click( , System.EventArgs.Empty); 
                return 1;
            }
            frm.lbloutput.Text = "InternetOpenAPi working";
            return 0;
        }

    }
}

