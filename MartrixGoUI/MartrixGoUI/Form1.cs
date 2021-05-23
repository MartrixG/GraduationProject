using System;
using System.Drawing;
using System.Text;
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

        public void Init(string Player1, string Player2, int Size)
        {
            BlackPlayerType = Player1;
            WhitePlayerType = Player2;
            BoardSize = Size;
        }

        private void MainWindow_Load(object sender, EventArgs e)
        {
            BoardPic = new PictureBox();
            if(BlackPlayerType == "ai")
            {
                NowAiPlayer = true;
            }
            if(BoardSize == 19)
            {
                ClientSize = new Size(1100, 920);
                StartBtn.Location = new Point(ClientSize.Width - 150, 20);
                CloseBtn.Location = new Point(ClientSize.Width - 150, 80);
                BoardPic.Image = Properties.Resources.棋盘19x19;
                BoardPic.Location = new Point(12, 12);
                BoardPic.Name = "BoardPic";
                BoardPic.Size = new Size(900, 900);
                BoardPic.SizeMode =PictureBoxSizeMode.StretchImage;
                BoardPic.TabIndex = 0;
                BoardPic.TabStop = false;
                Controls.Add(BoardPic);
                PointPic = new PictureBox[361];
                NowPlayerColor = 1;
                Board = new int[361];
                SendMsg = "";
                RecvBuf = new byte[512];
                for (int i = 0; i < 19; i++)
                {
                    for (int j = 0; j < 19; j++)
                    {
                        Board[i * 19 + j] = new int();
                        Board[i * 19 + j] = 0;
                        PointPic[i * 19 + j] = new PictureBox
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
            }
            else
            {
                ClientSize = new Size(640, 480);
                StartBtn.Location = new Point(ClientSize.Width - 150, 20);
                CloseBtn.Location = new Point(ClientSize.Width - 150, 80);
                BoardPic.Image = Properties.Resources.棋盘9x9;
                BoardPic.Location = new Point(12, 12);
                BoardPic.Name = "BoardPic";
                BoardPic.Size = new Size(460, 460);
                BoardPic.SizeMode = PictureBoxSizeMode.StretchImage;
                BoardPic.TabIndex = 0;
                BoardPic.TabStop = false;
                Controls.Add(BoardPic);
                PointPic = new PictureBox[81];
                NowPlayerColor = 1;
                Board = new int[81];
                SendMsg = "";
                RecvBuf = new byte[512];
                for (int i = 0; i < 9; i++)
                {
                    for (int j = 0; j < 9; j++)
                    {
                        Board[i * 9 + j] = new int();
                        Board[i * 9 + j] = 0;
                        PointPic[i * 9 + j] = new PictureBox
                        {
                            //Image = Properties.Resources.blackStone_65,
                            Location = new Point(23 + j * 46, 23 + i * 46),
                            Name = (i * 9 + j).ToString(),
                            Size = new Size(46, 46),
                            SizeMode = PictureBoxSizeMode.StretchImage,
                            Parent = BoardPic,
                            BackColor = Color.Transparent
                        };
                        BoardPic.Controls.Add(PointPic[i * 9 + j]);
                        PointPic[i * 9 + j].MouseEnter += new EventHandler(PointPic_MouseEnter);
                        PointPic[i * 9 + j].MouseLeave += new EventHandler(PointPic_MouseLeave);
                        PointPic[i * 9 + j].Click += new EventHandler(PointPic_Click);
                    }
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
            if (NowAiPlayer)
            {
                return;
            }
            PictureBox EventSender = (PictureBox)sender;
            int pos = int.Parse(EventSender.Name);
            if (Board[pos] == 0)
            {
                if (NowPlayerColor == 1)
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
            if (NowAiPlayer)
            {
                return;
            }
            PictureBox EventSender = (PictureBox)sender;
            int pos = int.Parse(EventSender.Name);
            if (Board[pos] == 0)
            {
                EventSender.Image = null;
            }
        }

        private void PointPic_Click(object sender, EventArgs e)
        {
            if(NowAiPlayer)
            {
                return;
            }
            PictureBox EventSender = (PictureBox)sender;
            int pos = int.Parse(EventSender.Name);
            if(Board[pos] == 0)
            {
                SendMsg = "n";
                SendMsg += EventSender.Name;
                SendMsg += '\0';
                ClientSocket.Send(Encoding.UTF8.GetBytes(SendMsg));
                ClientSocket.Receive(RecvBuf);
                int flag = Repaint(Encoding.UTF8.GetString(RecvBuf));
                if (flag != 1)
                {
                    MessageBox.Show("Expect board encode from backend.");
                    return;
                }
                if((NowPlayerColor == 1 && BlackPlayerType == "ai") || (NowPlayerColor == 2 && WhitePlayerType == "ai"))
                {
                    NowAiPlayer = true;
                    Thread thread = new(new ThreadStart(WaitForAi));
                    thread.IsBackground = true;
                    thread.Start();
                }
            }
        }

        private void StartBtn_Click(object sender, EventArgs e)
        {
            SendMsg = "s\0";
            ClientSocket.Send(Encoding.UTF8.GetBytes(SendMsg));
            if(BlackPlayerType == "ai")
            {
                ClientSocket.Receive(RecvBuf);
                int flag = Repaint(Encoding.UTF8.GetString(RecvBuf));
                if(flag != 1)
                {
                    MessageBox.Show("Expect board encode from backend.");
                    return;
                }
                if(WhitePlayerType != "ai")
                {
                    NowAiPlayer = false;
                }
                else
                {
                    Thread thread = new(new ThreadStart(WaitForAi));
                    thread.IsBackground = true;
                    thread.Start();
                }
            }
        }

        private void CloseBtn_Click(object sender, EventArgs e)
        {
            ClientSocket.Send(Encoding.UTF8.GetBytes("close"));
            StartBtn.Enabled = false;
        }

        private void WaitForAi()
        {
            ClientSocket.Receive(RecvBuf);
            int flag = Repaint(Encoding.UTF8.GetString(RecvBuf));
            if (flag != 1)
            {
                MessageBox.Show("Expect board encode from backend.");
                return;
            }
            if ((NowPlayerColor == 1 && BlackPlayerType == "ai") || (NowPlayerColor == 2 && WhitePlayerType == "ai"))
            {
                NowAiPlayer = true;
                WaitForAi();
            }
            else
            {
                NowAiPlayer = false;
            }
        }

        private int Repaint(string RecvBoard)
        {
            if(!RecvBoard.StartsWith('b'))
            {
               return 0;
            }
            NowPlayerColor = RecvBoard[1] - '0';
            for(int i = 0; i < BoardSize * BoardSize; i++)
            {
                if(Board[i] != RecvBoard[i + 2] - '0')
                {
                    Board[i] = RecvBoard[i + 2] - '0';
                    switch (Board[i])
                    {
                        case 0:
                            PointPic[i].Image = null;
                            break;
                        case 1:
                            PointPic[i].Image = Properties.Resources.blackStone;
                            break;
                        case 2:
                            PointPic[i].Image = Properties.Resources.whiteStone;
                            break;
                    }
                }
            }
            return 1;
        }
    }
}
