using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

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
            string backendGoArgs = "ui 127.0.0.1 23333 16";
            backendGoArgs += " " + StartMenu.BlackPlayerType;
            backendGoArgs += " " + StartMenu.BlackTime;
            backendGoArgs += " " + StartMenu.WhitePlayerType;
            backendGoArgs += " " + StartMenu.WhiteTime;
            Process ProcessExe = new();
            ProcessStartInfo ProInfo = new("E:/LEARNING/GraduationProject/MartrixGoC++/cmake-build-debug/MartrixGoC++.exe", backendGoArgs);
            ProInfo.CreateNoWindow = true;
            ProInfo.UseShellExecute = false;
            ProInfo.RedirectStandardOutput = true;
            ProcessExe.StartInfo = ProInfo;
            ProcessExe.Start();
            MainWindow MainWindow = new();
            MainWindow.Init(StartMenu.BlackPlayerType, StartMenu.WhitePlayerType, StartMenu.BoardSize);
            Application.Run(MainWindow);
            ProcessExe.WaitForExit();
            ProcessExe.Close();
        }
    }
}
