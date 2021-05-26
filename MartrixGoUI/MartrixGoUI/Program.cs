using System;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;

namespace MartrixGoUI
{
    static class Program
    {
        /// <summary>
        ///  The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.SetHighDpiMode(HighDpiMode.SystemAware);
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            StartForm StartMenu = new();
            Application.Run(StartMenu);
            if (!StartMenu.StartSucess)
                return;
            string backendGoArgs = "ui 127.0.0.1 23333";
            backendGoArgs += " " + StartMenu.ThreadNum;
            backendGoArgs += " " + StartMenu.BlackPlayerType;
            backendGoArgs += " " + StartMenu.BlackTime;
            backendGoArgs += " " + StartMenu.WhitePlayerType;
            backendGoArgs += " " + StartMenu.WhiteTime;
            Process BackendGoProcess = new();
            string LocalPath = Environment.CurrentDirectory;
            string GoBackend;
            if(StartMenu.BoardSize == 9)
            {
                GoBackend = "/MaritrixGoBackend9.exe";
            }
            else
            {
                GoBackend = "/MaritrixGoBackend19.exe";
            }
            if(!File.Exists(LocalPath + GoBackend))
            {
                MessageBox.Show("Please confirm the existence of MaritrixGoBackend.exe", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            ProcessStartInfo ProInfo = new(LocalPath + GoBackend, backendGoArgs);
            ProInfo.CreateNoWindow = true;
            ProInfo.UseShellExecute = false;
            ProInfo.RedirectStandardOutput = true;
            BackendGoProcess.StartInfo = ProInfo;
            BackendGoProcess.Start();
            MainWindow MainWindow = new();
            MainWindow.Init(StartMenu.BlackPlayerType, StartMenu.WhitePlayerType, StartMenu.BoardSize);
            Application.Run(MainWindow);
            BackendGoProcess.Kill();
        }
    }
}
