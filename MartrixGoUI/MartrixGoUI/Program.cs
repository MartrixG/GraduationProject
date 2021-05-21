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
            Form StartMenu = new Form2();
            Application.Run(StartMenu);

            //string backendGoArgs = "ui 127.0.0.0 23333 16 ai 10 human 0";
            //Process processexe = Process.Start("E:/LEARNING/GraduationProject/MartrixGoC++/cmake-build-debug/MartrixGoC++.exe", backendGoArgs);

            Form MainWindow = new MainWindow();
            Application.Run(MainWindow);
        }
    }
}
