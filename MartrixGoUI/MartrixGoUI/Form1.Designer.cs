
namespace MartrixGoUI
{
    partial class MainWindow
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainWindow));
            this.BoardPic = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.BoardPic)).BeginInit();
            this.SuspendLayout();
            // 
            // BoardPic
            // 
            this.BoardPic.Image = global::MartrixGoUI.Properties.Resources.棋盘;
            this.BoardPic.Location = new System.Drawing.Point(12, 12);
            this.BoardPic.Name = "BoardPic";
            this.BoardPic.Size = new System.Drawing.Size(900, 900);
            this.BoardPic.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.BoardPic.TabIndex = 0;
            this.BoardPic.TabStop = false;
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1198, 923);
            this.Controls.Add(this.BoardPic);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainWindow";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "MartrixGo";
            this.Load += new System.EventHandler(this.MainWindow_Load);
            ((System.ComponentModel.ISupportInitialize)(this.BoardPic)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox BoardPic;
        private System.Windows.Forms.PictureBox[] PointPic;
        private int player;
        private int[] board;
    }
}

