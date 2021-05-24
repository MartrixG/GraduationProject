
namespace MartrixGoUI
{
    partial class StartForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(StartForm));
            this.StartMain = new System.Windows.Forms.Button();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.boardSizeTextBox = new System.Windows.Forms.TextBox();
            this.BlackPayerChoseTextBox = new System.Windows.Forms.TextBox();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.WhitePlayerTextBox = new System.Windows.Forms.TextBox();
            this.comboBox3 = new System.Windows.Forms.ComboBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.comboBox4 = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // StartMain
            // 
            this.StartMain.Location = new System.Drawing.Point(144, 174);
            this.StartMain.Name = "StartMain";
            this.StartMain.Size = new System.Drawing.Size(93, 29);
            this.StartMain.TabIndex = 0;
            this.StartMain.Text = "开始";
            this.StartMain.UseVisualStyleBackColor = true;
            this.StartMain.Click += new System.EventHandler(this.StartMain_Click);
            // 
            // comboBox1
            // 
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            "请选择棋盘大小...",
            "9 x 9",
            "19 x 19"});
            this.comboBox1.Location = new System.Drawing.Point(208, 22);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(154, 28);
            this.comboBox1.TabIndex = 1;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.ComboBox1_SelectedIndexChanged);
            // 
            // boardSizeTextBox
            // 
            this.boardSizeTextBox.Font = new System.Drawing.Font("Microsoft YaHei UI", 10.28571F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.boardSizeTextBox.Location = new System.Drawing.Point(27, 22);
            this.boardSizeTextBox.Name = "boardSizeTextBox";
            this.boardSizeTextBox.Size = new System.Drawing.Size(144, 29);
            this.boardSizeTextBox.TabIndex = 2;
            this.boardSizeTextBox.Text = "棋盘大小";
            this.boardSizeTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // BlackPayerChoseTextBox
            // 
            this.BlackPayerChoseTextBox.Font = new System.Drawing.Font("Microsoft YaHei UI", 10.28571F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.BlackPayerChoseTextBox.Location = new System.Drawing.Point(27, 57);
            this.BlackPayerChoseTextBox.Name = "BlackPayerChoseTextBox";
            this.BlackPayerChoseTextBox.Size = new System.Drawing.Size(144, 29);
            this.BlackPayerChoseTextBox.TabIndex = 3;
            this.BlackPayerChoseTextBox.Text = "黑方棋手";
            this.BlackPayerChoseTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // comboBox2
            // 
            this.comboBox2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Items.AddRange(new object[] {
            "请选择黑方棋手...",
            "AI(Simple)",
            "AI(Medium)",
            "AI(Hard)",
            "玩家"});
            this.comboBox2.Location = new System.Drawing.Point(208, 57);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(154, 28);
            this.comboBox2.TabIndex = 4;
            this.comboBox2.SelectedIndexChanged += new System.EventHandler(this.ComboBox2_SelectedIndexChanged);
            // 
            // WhitePlayerTextBox
            // 
            this.WhitePlayerTextBox.Font = new System.Drawing.Font("Microsoft YaHei UI", 10.28571F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.WhitePlayerTextBox.Location = new System.Drawing.Point(27, 92);
            this.WhitePlayerTextBox.Name = "WhitePlayerTextBox";
            this.WhitePlayerTextBox.Size = new System.Drawing.Size(144, 29);
            this.WhitePlayerTextBox.TabIndex = 5;
            this.WhitePlayerTextBox.Text = "白方棋手";
            this.WhitePlayerTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // comboBox3
            // 
            this.comboBox3.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox3.FormattingEnabled = true;
            this.comboBox3.Items.AddRange(new object[] {
            "请选择白方棋手...",
            "AI(Simple)",
            "AI(Medium)",
            "AI(Hard)",
            "玩家"});
            this.comboBox3.Location = new System.Drawing.Point(208, 91);
            this.comboBox3.Name = "comboBox3";
            this.comboBox3.Size = new System.Drawing.Size(154, 28);
            this.comboBox3.TabIndex = 6;
            this.comboBox3.SelectedIndexChanged += new System.EventHandler(this.ComboBox3_SelectedIndexChanged);
            // 
            // textBox1
            // 
            this.textBox1.Font = new System.Drawing.Font("Microsoft YaHei UI", 10.28571F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.textBox1.Location = new System.Drawing.Point(27, 128);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(144, 29);
            this.textBox1.TabIndex = 7;
            this.textBox1.Text = "搜索线程数";
            this.textBox1.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // comboBox4
            // 
            this.comboBox4.FormattingEnabled = true;
            this.comboBox4.Location = new System.Drawing.Point(208, 128);
            this.comboBox4.Name = "comboBox4";
            this.comboBox4.Size = new System.Drawing.Size(154, 28);
            this.comboBox4.TabIndex = 8;
            // 
            // StartForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(384, 215);
            this.Controls.Add(this.comboBox4);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.comboBox3);
            this.Controls.Add(this.WhitePlayerTextBox);
            this.Controls.Add(this.comboBox2);
            this.Controls.Add(this.BlackPayerChoseTextBox);
            this.Controls.Add(this.boardSizeTextBox);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.StartMain);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "StartForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "MartrixGo";
            this.Load += new System.EventHandler(this.Form2_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button StartMain;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.TextBox boardSizeTextBox;
        private System.Windows.Forms.TextBox BlackPayerChoseTextBox;
        private System.Windows.Forms.ComboBox comboBox2;
        private System.Windows.Forms.TextBox WhitePlayerTextBox;
        private System.Windows.Forms.ComboBox comboBox3;

        public bool StartSucess = false;
        public int BoardSize;
        public int BlackPlayer;
        public int WhitePlayer;
        public string BlackPlayerType;
        public string BlackTime;
        public string WhitePlayerType;
        public string WhiteTime;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.ComboBox comboBox4;
    }
}