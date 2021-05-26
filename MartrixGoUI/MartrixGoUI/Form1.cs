using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.Threading.Tasks;

namespace MartrixGoUI
{
    public partial class MainWindow : Form
    {
        delegate void AsynUpdateUI(string RecvBoard);

        public MainWindow()
        {
            InitializeComponent();
        }

        public void Init(string Player1, string Player2, int Size)
        {
            BlackPlayerType = Player1;
            WhitePlayerType = Player2;
            BoardSize = Size;
            NowAiPlayer = true;
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
                Text += ": " + (BlackPlayerType == "ai" ? "AI" : "玩家") + " 对 " + (WhitePlayerType == "ai" ? "AI" : "玩家");
                InformaitionLabel.Location = new Point(12, 10);
                InformaitionLabel.Size = new Size(900, 27);
                ClientSize = new Size(1080, 950);

                StartBtn.Location = new Point(ClientSize.Width - 150, 15 + 25 + 5);
                pictureBox1.Location = new Point(ClientSize.Width - 150, 40 + 29 + 15);
                label1.Location = new Point(ClientSize.Width - 150 + 88, 40 + 29 + 15);
                label1.Text = (BlackPlayerType == "ai" ? "AI" : "玩家");
                pictureBox2.Location = new Point(ClientSize.Width - 150, 40 + 29 + 15 + 29 + 15);
                label2.Location = new Point(ClientSize.Width - 150 + 88, 40 + 29 + 15 + 29 + 15);
                label2.Text = (WhitePlayerType == "ai" ? "AI" : "玩家");

                BoardPic.Image = Properties.Resources.棋盘19x19;
                BoardPic.Location = new Point(12, 12 + 25 + 5);
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
                Text += ": " + (BlackPlayerType == "ai" ? "AI" : "玩家") + " 对 " + (WhitePlayerType == "ai" ? "AI" : "玩家");
                InformaitionLabel.Location = new Point(12, 10);
                InformaitionLabel.Size = new Size(460, 27);
                ClientSize = new Size(640, 505);

                StartBtn.Location = new Point(ClientSize.Width - 150, 10 + 25 + 5);
                pictureBox1.Location = new Point(ClientSize.Width - 150, 40 + 29 + 10);
                label1.Location = new Point(ClientSize.Width - 150 + 88, 40 + 29 + 10);
                label1.Text = (BlackPlayerType == "ai" ? "AI" : "玩家");
                pictureBox2.Location = new Point(ClientSize.Width - 150, 40 + 29 + 10 + 29 + 10);
                label2.Location = new Point(ClientSize.Width - 150 + 88, 40 + 29 + 10 + 29 + 10);
                label2.Text = (WhitePlayerType == "ai" ? "AI" : "玩家");

                BoardPic.Image = Properties.Resources.棋盘9x9;
                BoardPic.Location = new Point(12, 10 + 25 + 5);
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
            StepNum = 0;
            IPEndPoint point = new(IPAddress.Parse("127.0.0.1"), 23333);
            ClientSocket = new(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                ClientSocket.Connect(point);
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Connect error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Close();
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
                Repaint(Encoding.UTF8.GetString(RecvBuf));
                if (RecvMsgProcessCode != 1)
                {
                    MessageBox.Show("Expect board encode from backend.");
                    return;
                }
                if((NowPlayerColor == 1 && BlackPlayerType == "ai") || (NowPlayerColor == 2 && WhitePlayerType == "ai"))
                {
                    NowAiPlayer = true;
                    using BackgroundWorker bw = new();
                    bw.DoWork += new DoWorkEventHandler(WaitingForAi);
                    bw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(WaitForAi);
                    bw.RunWorkerAsync();
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
                Repaint(Encoding.UTF8.GetString(RecvBuf));
                if (WhitePlayerType != "ai")
                {
                    NowAiPlayer = false;
                }
                else
                {
                    using BackgroundWorker bw = new();
                    bw.DoWork += new DoWorkEventHandler(WaitingForAi);
                    bw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(WaitForAi);
                    bw.RunWorkerAsync();
                }
            }
            else
            {
                NowAiPlayer = false;
            }
            StartBtn.Enabled = false;
        }

        private void WaitingForAi(object sender, DoWorkEventArgs e)
        {
            try
            {
                ClientSocket.Receive(RecvBuf);
            }
            catch (Exception Ex)
            {
                Ex.ToString();
            }
        }

        private void WaitForAi(object sender, RunWorkerCompletedEventArgs e)
        {
            Repaint(Encoding.UTF8.GetString(RecvBuf));
            switch (RecvMsgProcessCode)
            {
                case 0:
                    MessageBox.Show("Expect board encode from backend.");
                    return;
                case 1:
                    if ((NowPlayerColor == 1 && BlackPlayerType == "ai") || (NowPlayerColor == 2 && WhitePlayerType == "ai"))
                    {
                        NowAiPlayer = true;
                        using BackgroundWorker bw = new();
                        bw.DoWork += new DoWorkEventHandler(WaitingForAi);
                        bw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(WaitForAi);
                        bw.RunWorkerAsync();
                    }
                    else
                    {
                        NowAiPlayer = false;
                    }
                    break;
                case 2:
                    MessageBox.Show(NowPlayerColor == 1 ? "Black player confess." : "White player confess.", "Confess", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    NowAiPlayer = true;
                    break;
            }
        }

        private void Repaint(string RecvBoard)
        {
            if (RecvBoard.StartsWith('c'))
            {
                RecvMsgProcessCode = 2;
                return;
            }
            if(!RecvBoard.StartsWith('b'))
            {
                RecvMsgProcessCode = 0;
                return;
            }
            for(int i = 0; i < BoardSize * BoardSize; i++)
            {
                if(Board[i] != RecvBoard[i + 2] - '0')
                {
                    StepNum++;
                    Board[i] = RecvBoard[i + 2] - '0';
                    if (Board[i] == NowPlayerColor)
                    {
                        LastMovePos = i;
                    }
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
            NowPlayerColor = RecvBoard[1] - '0';
            RecvMsgProcessCode = 1;

            InformaitionLabel.Text = "第" + StepNum.ToString() + "手" + "(" + (NowPlayerColor == 1 ? "白" : "黑");
            int x = LastMovePos / BoardSize;
            x = BoardSize - x;
            char y = (char)('A' + LastMovePos % BoardSize);
            if (y == 'I') y = 'J';
            InformaitionLabel.Text += (y + x.ToString() + "):该" + (NowPlayerColor == 1 ? "黑" : "白") + "下");
        }


    }
}
