using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;
using System.Threading;

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
            PointPic = new PictureBox[361];
            Player = 1;
            HandCapFlag = false;
            Board = new int[361];
            SendMsg = "";
            RecvBuf = new byte[1024];
            for (int i = 0; i < 19; i++)
            {
                for (int j = 0; j < 19; j++)
                {
                    Board[i * 19 + j] = new int();
                    Board[i * 19 + j] = 0;
                    this.PointPic[i * 19 + j] = new System.Windows.Forms.PictureBox
                    {
                        //Image = Properties.Resources.blackStone_65,
                        Location = new Point(12 + j * 46, 14 + i * 46),
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
            IPEndPoint point = new(IPAddress.Parse("127.0.0.1"), 23333);
            ClientSocket = new(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                ClientSocket.Connect(point);
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        private void PointPic_MouseEnter(object sender, EventArgs e)
        {
            PictureBox EventSender = (PictureBox)sender;
            int pos = int.Parse(EventSender.Name);
            if (Board[pos] == 0)
            {
                if (Player == 1)
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
            if (Board[pos] == 0)
            {
                EventSender.Image = null;
            }
        }

        private void PointPic_Click(object sender, EventArgs e)
        {
            PictureBox EventSender = (PictureBox)sender;
            int pos = int.Parse(EventSender.Name);

            if(HandCapFlag == false)
            {
                if(Board[pos] == 0)
                {
                    if (Player == 1)
                    {
                        EventSender.Image = Properties.Resources.blackStone;
                        Board[pos] = 1;
                    }
                    else
                    {
                        EventSender.Image = Properties.Resources.whiteStone;
                        Board[pos] = 2;
                    }
                }
            }
            else
            {
                if(Board[pos] == 0)
                {
                    SendMsg = "s";
                    SendMsg += EventSender.Name;
                    SendMsg += '\0';
                    ClientSocket.Send(Encoding.UTF8.GetBytes(SendMsg));
                    ClientSocket.Receive(RecvBuf);
                    Repaint(Encoding.UTF8.GetString(RecvBuf));
                }
            }
        }

        private void SocketBtn_Click(object sender, EventArgs e)
        {
            SendMsg = "s\0";
            ClientSocket.Send(Encoding.UTF8.GetBytes(SendMsg));
            string recv = "e";
            while (recv.StartsWith("e"))
            {
                ClientSocket.Receive(RecvBuf);
                recv = Encoding.UTF8.GetString(RecvBuf);
            }
            Repaint(recv);
        }

        private void CloseBtn_Click(object sender, EventArgs e)
        {
            ClientSocket.Send(Encoding.UTF8.GetBytes("close"));
            this.SocketBtn.Enabled = false;
        }

        private void Repaint(string RecvBoard)
        {
            Player = RecvBoard[1] - '0';
            for(int i = 1; i < 362; i++)
            {
                if(Board[i - 1] != RecvBoard[i] - '0')
                {
                    Board[i - 1] = RecvBoard[i] - '0';
                    switch (Board[i - 1])
                    {
                        case 0:
                            PointPic[i - 1].Image = null;
                            break;
                        case 1:
                            PointPic[i - 1].Image = Properties.Resources.blackStone;
                            break;
                        case 2:
                            PointPic[i - 1].Image = Properties.Resources.whiteStone;
                            break;
                    }
                }
            }
        }
    }
}
