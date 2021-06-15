
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
            this.SearchThreadTextBox = new System.Windows.Forms.TextBox();
            this.comboBox4 = new System.Windows.Forms.ComboBox();
            this.searchTimeBox1 = new System.Windows.Forms.TextBox();
            this.comboBox5 = new System.Windows.Forms.ComboBox();
            this.comboBox6 = new System.Windows.Forms.ComboBox();
            this.searchTImeTextBox2 = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // StartMain
            // 
            this.StartMain.Location = new System.Drawing.Point(139, 233);
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
            this.boardSizeTextBox.ReadOnly = true;
            this.boardSizeTextBox.Size = new System.Drawing.Size(144, 29);
            this.boardSizeTextBox.TabIndex = 2;
            this.boardSizeTextBox.Text = "棋盘大小";
            this.boardSizeTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // BlackPayerChoseTextBox
            // 
            this.BlackPayerChoseTextBox.Font = new System.Drawing.Font("Microsoft YaHei UI", 10.28571F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.BlackPayerChoseTextBox.Location = new System.Drawing.Point(27, 91);
            this.BlackPayerChoseTextBox.Name = "BlackPayerChoseTextBox";
            this.BlackPayerChoseTextBox.ReadOnly = true;
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
            this.comboBox2.Location = new System.Drawing.Point(208, 91);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(154, 28);
            this.comboBox2.TabIndex = 4;
            this.comboBox2.SelectedIndexChanged += new System.EventHandler(this.ComboBox2_SelectedIndexChanged);
            // 
            // WhitePlayerTextBox
            // 
            this.WhitePlayerTextBox.Font = new System.Drawing.Font("Microsoft YaHei UI", 10.28571F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.WhitePlayerTextBox.Location = new System.Drawing.Point(27, 162);
            this.WhitePlayerTextBox.Name = "WhitePlayerTextBox";
            this.WhitePlayerTextBox.ReadOnly = true;
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
            this.comboBox3.Location = new System.Drawing.Point(208, 161);
            this.comboBox3.Name = "comboBox3";
            this.comboBox3.Size = new System.Drawing.Size(154, 28);
            this.comboBox3.TabIndex = 6;
            this.comboBox3.SelectedIndexChanged += new System.EventHandler(this.ComboBox3_SelectedIndexChanged);
            // 
            // SearchThreadTextBox
            // 
            this.SearchThreadTextBox.Font = new System.Drawing.Font("Microsoft YaHei UI", 10.28571F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.SearchThreadTextBox.Location = new System.Drawing.Point(27, 57);
            this.SearchThreadTextBox.Name = "SearchThreadTextBox";
            this.SearchThreadTextBox.ReadOnly = true;
            this.SearchThreadTextBox.Size = new System.Drawing.Size(144, 29);
            this.SearchThreadTextBox.TabIndex = 7;
            this.SearchThreadTextBox.Text = "搜索线程数";
            this.SearchThreadTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // comboBox4
            // 
            this.comboBox4.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox4.Font = new System.Drawing.Font("Microsoft YaHei UI", 9.07563F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.comboBox4.FormattingEnabled = true;
            this.comboBox4.Items.AddRange(new object[] {
            "请选择搜索线程数...",
            "1",
            "2",
            "4",
            "8",
            "10",
            "16"});
            this.comboBox4.Location = new System.Drawing.Point(208, 57);
            this.comboBox4.Name = "comboBox4";
            this.comboBox4.Size = new System.Drawing.Size(154, 28);
            this.comboBox4.TabIndex = 8;
            this.comboBox4.SelectedIndexChanged += new System.EventHandler(this.ComboBox4_SelectedIndexChanged);
            // 
            // searchTimeBox1
            // 
            this.searchTimeBox1.Font = new System.Drawing.Font("Microsoft YaHei UI", 10.28571F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.searchTimeBox1.Location = new System.Drawing.Point(27, 127);
            this.searchTimeBox1.Name = "searchTimeBox1";
            this.searchTimeBox1.ReadOnly = true;
            this.searchTimeBox1.Size = new System.Drawing.Size(144, 29);
            this.searchTimeBox1.TabIndex = 9;
            this.searchTimeBox1.Text = "搜索时间";
            this.searchTimeBox1.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // comboBox5
            // 
            this.comboBox5.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox5.FormattingEnabled = true;
            this.comboBox5.Items.AddRange(new object[] {
            "请选择搜索时间...",
            "10s",
            "15s",
            "20s",
            "25s",
            "30s"});
            this.comboBox5.Location = new System.Drawing.Point(208, 127);
            this.comboBox5.Name = "comboBox5";
            this.comboBox5.Size = new System.Drawing.Size(154, 28);
            this.comboBox5.TabIndex = 10;
            this.comboBox5.SelectedIndexChanged += new System.EventHandler(this.comboBox5_SelectedIndexChanged);
            // 
            // comboBox6
            // 
            this.comboBox6.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox6.FormattingEnabled = true;
            this.comboBox6.Items.AddRange(new object[] {
            "请选择搜索时间...",
            "10s",
            "15s",
            "20s",
            "25s",
            "30s"});
            this.comboBox6.Location = new System.Drawing.Point(208, 196);
            this.comboBox6.Name = "comboBox6";
            this.comboBox6.Size = new System.Drawing.Size(154, 28);
            this.comboBox6.TabIndex = 11;
            this.comboBox6.SelectedIndexChanged += new System.EventHandler(this.comboBox6_SelectedIndexChanged);
            // 
            // searchTImeTextBox2
            // 
            this.searchTImeTextBox2.Font = new System.Drawing.Font("Microsoft YaHei UI", 10.28571F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.searchTImeTextBox2.Location = new System.Drawing.Point(27, 197);
            this.searchTImeTextBox2.Name = "searchTImeTextBox2";
            this.searchTImeTextBox2.ReadOnly = true;
            this.searchTImeTextBox2.Size = new System.Drawing.Size(144, 29);
            this.searchTImeTextBox2.TabIndex = 12;
            this.searchTImeTextBox2.Text = "搜索时间";
            this.searchTImeTextBox2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // StartForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(388, 274);
            this.Controls.Add(this.searchTImeTextBox2);
            this.Controls.Add(this.comboBox6);
            this.Controls.Add(this.comboBox5);
            this.Controls.Add(this.searchTimeBox1);
            this.Controls.Add(this.comboBox4);
            this.Controls.Add(this.SearchThreadTextBox);
            this.Controls.Add(this.comboBox3);
            this.Controls.Add(this.WhitePlayerTextBox);
            this.Controls.Add(this.comboBox2);
            this.Controls.Add(this.BlackPayerChoseTextBox);
            this.Controls.Add(this.boardSizeTextBox);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.StartMain);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
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
        private System.Windows.Forms.TextBox SearchThreadTextBox;
        private System.Windows.Forms.ComboBox comboBox4;

        public bool StartSucess = false;
        public int BoardSize;
        public int BlackPlayer;
        public int WhitePlayer;

        public int ThreadNumCode;
        public string ThreadNum;

        public string BlackPlayerType;
        public int BlackPlayerSearchTimeCode;
        public string BlackTime;

        public string WhitePlayerType;
        public int WhitePlayerSearchTimeCode;
        public string WhiteTime;
        
        private System.Windows.Forms.TextBox searchTimeBox1;
        private System.Windows.Forms.ComboBox comboBox5;
        private System.Windows.Forms.ComboBox comboBox6;
        private System.Windows.Forms.TextBox searchTImeTextBox2;
    }
}