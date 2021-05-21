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
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        private void StartMain_Click(object sender, EventArgs e)
        {
            if(BoardSize * BlackPlayer * WhitePlayer == 0)
            {
                MessageBox.Show("请确保三个选项均已被选择");
            }
            Close();
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
        }
        private void ComboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            BlackPlayer = comboBox2.SelectedIndex;
        }

        private void ComboBox3_SelectedIndexChanged(object sender, EventArgs e)
        {
            WhitePlayer = comboBox3.SelectedIndex;
        }
    }
}
