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
            if (BoardSize * BlackPlayer * WhitePlayer == 0)
            {
                MessageBox.Show("请确保三个选项均已被选择");
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
                    BlackTime = "10";
                    break;
                case 2:
                    BlackPlayerType = "ai";
                    BlackTime = "20";
                    break;
                case 3:
                    BlackPlayerType = "ai";
                    BlackTime = "30";
                    break;
                case 4:
                    BlackPlayerType = "human";
                    BlackTime = "0";
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
                    WhiteTime = "10";
                    break;
                case 2:
                    WhitePlayerType = "ai";
                    WhiteTime = "20";
                    break;
                case 3:
                    WhitePlayerType = "ai";
                    WhiteTime = "30";
                    break;
                case 4:
                    WhitePlayerType = "human";
                    WhiteTime = "0";
                    break;
            }
        }
    }
}
