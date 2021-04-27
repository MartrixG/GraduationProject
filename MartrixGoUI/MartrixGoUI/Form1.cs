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
    public partial class MainWindow : Form
    {
        public MainWindow()
        {
            InitializeComponent();
        }
        private void MainWindow_Load(object sender, EventArgs e)
        {
            player = 1;
            board = new int[361];
            PointPic = new PictureBox[361];
            for (int i = 0; i < 19; i++)
            {
                for (int j = 0; j < 19; j++)
                {
                    board[i * 19 + j] = new int();
                    board[i * 19 + j] = 0;
                    this.PointPic[i * 19 + j] = new System.Windows.Forms.PictureBox
                    {
                        //Image = Properties.Resources.blackStone_65,
                        Location = new Point(12 + i * 46, 14 + j * 46),
                        Name = (i * 19 + j).ToString(),
                        Size = new Size(46, 46),
                        SizeMode = PictureBoxSizeMode.StretchImage,
                        Parent = BoardPic,
                        BackColor = Color.Transparent
                    };
                    BoardPic.Controls.Add(PointPic[i * 19 + j]);
                    PointPic[i * 19 + j].MouseEnter += new EventHandler(PointPic_MouseEnter);
                    PointPic[i * 19 + j].MouseLeave += new EventHandler(PointPic_MouseLeave);
                    PointPic[i * 19 + j].Click += new EventHandler(PointPic_Click);
                }
            }
        }

        private void PointPic_MouseEnter(object sender, EventArgs e)
        {
            PictureBox EventSender = (PictureBox)sender;
            int pos = int.Parse(EventSender.Name);
            if (board[pos] == 0)
            {
                if (player == 1)
                {
                    EventSender.Image = Properties.Resources.blackStone_75;
                }
                else
                {
                    EventSender.Image = Properties.Resources.whiteStone_75;
                }
            }
        }

        private void PointPic_MouseLeave(object sender, EventArgs e)
        {
            PictureBox EventSender = (PictureBox)sender;
            int pos = int.Parse(EventSender.Name);
            if (board[pos] == 0)
            {
                EventSender.Image = null;
            }
        }

        private void PointPic_Click(object sender, EventArgs e)
        {
            PictureBox EventSender = (PictureBox)sender;
            int pos = int.Parse(EventSender.Name);
            if(board[pos] == 0)
            {
                if (player == 1)
                {
                    EventSender.Image = Properties.Resources.blackStone;
                    board[pos] = 1;
                }
                else
                {
                    EventSender.Image = Properties.Resources.whiteStone;
                    board[pos] = 2;
                }
                player = player == 1 ? 2 : 1;
            }
        }
    }
}
