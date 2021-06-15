using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MartrixGoUI
{
    public partial class StartForm: Form
    {
        public StartForm()
        {
            InitializeComponent();
        }

        private void StartMain_Click(object sender, EventArgs e)
        {
            if (BoardSize * BlackPlayer * WhitePlayer * ThreadNumCode == 0)
            {
                MessageBox.Show("请确保所有选项均已被选择");
            }
            else if((BlackPlayerType == "ai" && BlackPlayerSearchTimeCode == 0) || (WhitePlayerType == "ai" && WhitePlayerSearchTimeCode == 0))
            {
                MessageBox.Show("请确保所有选项均已被选择");
            }
            else
            {
                StartSucess = true;
                Close();
            }
        }


        private void Form2_Load(object sender, EventArgs e)
        {
            comboBox1.Text = "请选择棋盘大小...";
            comboBox2.Text = "请选择黑方棋手...";
            comboBox3.Text = "请选择白方棋手...";
            comboBox4.Text = "请选择搜索线程数...";
            comboBox5.Text = "请选择搜索时间...";
            comboBox5.Enabled = false;
            comboBox6.Text = "请选择搜索时间...";
            comboBox6.Enabled = false;
        }
        private void ComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            BoardSize = comboBox1.SelectedIndex;
            if (BoardSize == 1)
                BoardSize = 9;
            if (BoardSize == 2)
                BoardSize = 19;
        }
        private void ComboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            BlackPlayer = comboBox2.SelectedIndex;
            switch(BlackPlayer)
            {
                case 1:
                    BlackPlayerType = "ai";
                    BlackTime = "1";
                    comboBox5.Enabled = true;
                    break;
                case 2:
                    BlackPlayerType = "ai";
                    BlackTime = "2";
                    comboBox5.Enabled = true;
                    break;
                case 3:
                    BlackPlayerType = "ai";
                    BlackTime = "3";
                    comboBox5.Enabled = true;
                    break;
                case 4:
                    BlackPlayerType = "human";
                    BlackTime = "0";
                    comboBox5.Text = "请选择搜索时间...";
                    comboBox5.Enabled = false;
                    break;
            }
        }

        private void ComboBox3_SelectedIndexChanged(object sender, EventArgs e)
        {
            WhitePlayer = comboBox3.SelectedIndex;
            switch (WhitePlayer)
            {
                case 1:
                    WhitePlayerType = "ai";
                    WhiteTime = "1";
                    comboBox6.Enabled = true;
                    break;
                case 2:
                    WhitePlayerType = "ai";
                    WhiteTime = "2";
                    comboBox6.Enabled = true;
                    break;
                case 3:
                    WhitePlayerType = "ai";
                    WhiteTime = "3";
                    comboBox6.Enabled = true;
                    break;
                case 4:
                    WhitePlayerType = "human";
                    WhiteTime = "0";
                    comboBox6.Text = "请选择搜索时间...";
                    comboBox6.Enabled = false;
                    break;
            }
        }

        private void ComboBox4_SelectedIndexChanged(object sender, EventArgs e)
        {
            ThreadNumCode = comboBox4.SelectedIndex;
            switch(ThreadNumCode)
            {
                case 1:
                    ThreadNum = "1";
                    break;
                case 2:
                    ThreadNum = "2";
                    break;
                case 3:
                    ThreadNum = "4";
                    break;
                case 4:
                    ThreadNum = "8";
                    break;
                case 5:
                    ThreadNum = "10";
                    break;
                case 6:
                    ThreadNum = "16";
                    break;
            }
        }

        private void comboBox5_SelectedIndexChanged(object sender, EventArgs e)
        {
            BlackPlayerSearchTimeCode = comboBox5.SelectedIndex;
            if (BlackPlayerSearchTimeCode == 0)
                return;
            BlackTime = BlackTime.Substring(BlackTime.Length - 1, 1);
            switch (BlackPlayerSearchTimeCode)
            {
                case 1:
                    BlackTime = "1000" + BlackTime;
                    break;
                case 2:
                    BlackTime = "1500" + BlackTime;
                    break;
                case 3:
                    BlackTime = "2000" + BlackTime;
                    break;
                case 4:
                    BlackTime = "2500" + BlackTime;
                    break;
                case 5:
                    BlackTime = "3000" + BlackTime;
                    break;
            }
        }

        private void comboBox6_SelectedIndexChanged(object sender, EventArgs e)
        {
            WhitePlayerSearchTimeCode = comboBox6.SelectedIndex;
            if (WhitePlayerSearchTimeCode == 0)
                return;
            WhiteTime = WhiteTime.Substring(WhiteTime.Length - 1, 1);
            switch(WhitePlayerSearchTimeCode)
            {
                case 1:
                    WhiteTime = "1000" + WhiteTime;
                    break;
                case 2:
                    WhiteTime = "1500" + WhiteTime;
                    break;
                case 3:
                    WhiteTime = "2000" + WhiteTime;
                    break;
                case 4:
                    WhiteTime = "2500" + WhiteTime;
                    break;
                case 5:
                    WhiteTime = "3000" + WhiteTime;
                    break;
            }
        }
    }
}
