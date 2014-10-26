namespace UploadToFlash
{
    partial class Main
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
      this._filesList = new System.Windows.Forms.ListView();
      this.label1 = new System.Windows.Forms.Label();
      this._nextAddress = new System.Windows.Forms.TextBox();
      this._btnAdd = new System.Windows.Forms.Button();
      this.label2 = new System.Windows.Forms.Label();
      this.label3 = new System.Windows.Forms.Label();
      this._baudRate = new System.Windows.Forms.TextBox();
      this._serialPort = new System.Windows.Forms.ComboBox();
      this._btnRemove = new System.Windows.Forms.Button();
      this._btnProgram = new System.Windows.Forms.Button();
      this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
      this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
      this.SuspendLayout();
      // 
      // _filesList
      // 
      this._filesList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this._filesList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
      this._filesList.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
      this._filesList.Location = new System.Drawing.Point(12, 12);
      this._filesList.Name = "_filesList";
      this._filesList.Size = new System.Drawing.Size(499, 182);
      this._filesList.TabIndex = 0;
      this._filesList.UseCompatibleStateImageBehavior = false;
      this._filesList.View = System.Windows.Forms.View.Details;
      this._filesList.SelectedIndexChanged += new System.EventHandler(this._filesList_SelectedIndexChanged);
      this._filesList.DoubleClick += new System.EventHandler(this._filesList_DoubleClick);
      // 
      // label1
      // 
      this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(12, 207);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(108, 13);
      this.label1.TabIndex = 1;
      this.label1.Text = "&Next address in flash:";
      // 
      // _nextAddress
      // 
      this._nextAddress.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this._nextAddress.Location = new System.Drawing.Point(126, 204);
      this._nextAddress.Name = "_nextAddress";
      this._nextAddress.Size = new System.Drawing.Size(85, 20);
      this._nextAddress.TabIndex = 2;
      this._nextAddress.Text = "0";
      // 
      // _btnAdd
      // 
      this._btnAdd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this._btnAdd.Location = new System.Drawing.Point(230, 204);
      this._btnAdd.Name = "_btnAdd";
      this._btnAdd.Size = new System.Drawing.Size(130, 42);
      this._btnAdd.TabIndex = 7;
      this._btnAdd.Text = "&Add images...";
      this._btnAdd.UseVisualStyleBackColor = true;
      this._btnAdd.Click += new System.EventHandler(this._btnAdd_Click);
      // 
      // label2
      // 
      this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(12, 233);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(94, 13);
      this.label2.TabIndex = 3;
      this.label2.Text = "Arduino &serial port:";
      // 
      // label3
      // 
      this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this.label3.AutoSize = true;
      this.label3.Location = new System.Drawing.Point(12, 262);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(84, 13);
      this.label3.TabIndex = 5;
      this.label3.Text = "Serial &baud rate:";
      // 
      // _baudRate
      // 
      this._baudRate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this._baudRate.Location = new System.Drawing.Point(126, 259);
      this._baudRate.Name = "_baudRate";
      this._baudRate.Size = new System.Drawing.Size(85, 20);
      this._baudRate.TabIndex = 6;
      this._baudRate.Text = "115200";
      // 
      // _serialPort
      // 
      this._serialPort.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this._serialPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this._serialPort.FormattingEnabled = true;
      this._serialPort.Location = new System.Drawing.Point(126, 230);
      this._serialPort.Name = "_serialPort";
      this._serialPort.Size = new System.Drawing.Size(85, 21);
      this._serialPort.TabIndex = 4;
      // 
      // _btnRemove
      // 
      this._btnRemove.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this._btnRemove.Location = new System.Drawing.Point(230, 252);
      this._btnRemove.Name = "_btnRemove";
      this._btnRemove.Size = new System.Drawing.Size(130, 27);
      this._btnRemove.TabIndex = 8;
      this._btnRemove.Text = "&Remove selected";
      this._btnRemove.UseVisualStyleBackColor = true;
      this._btnRemove.Click += new System.EventHandler(this._btnRemove_Click);
      // 
      // _btnProgram
      // 
      this._btnProgram.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this._btnProgram.Location = new System.Drawing.Point(381, 204);
      this._btnProgram.Name = "_btnProgram";
      this._btnProgram.Size = new System.Drawing.Size(130, 75);
      this._btnProgram.TabIndex = 9;
      this._btnProgram.Text = "&Program now";
      this._btnProgram.UseVisualStyleBackColor = true;
      this._btnProgram.Click += new System.EventHandler(this._btnProgram_Click);
      // 
      // columnHeader1
      // 
      this.columnHeader1.Text = "Pathname";
      this.columnHeader1.Width = 354;
      // 
      // columnHeader2
      // 
      this.columnHeader2.Text = "Flash address";
      this.columnHeader2.Width = 116;
      // 
      // Main
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(527, 292);
      this.Controls.Add(this._btnProgram);
      this.Controls.Add(this._btnRemove);
      this.Controls.Add(this._serialPort);
      this.Controls.Add(this.label3);
      this.Controls.Add(this.label2);
      this.Controls.Add(this._btnAdd);
      this.Controls.Add(this._baudRate);
      this.Controls.Add(this._nextAddress);
      this.Controls.Add(this.label1);
      this.Controls.Add(this._filesList);
      this.Name = "Main";
      this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
      this.Text = "Upload Images to flash";
      this.Load += new System.EventHandler(this.Main_Load);
      this.ResumeLayout(false);
      this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView _filesList;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox _nextAddress;
        private System.Windows.Forms.Button _btnAdd;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox _baudRate;
        private System.Windows.Forms.ComboBox _serialPort;
        private System.Windows.Forms.Button _btnRemove;
        private System.Windows.Forms.Button _btnProgram;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
    }
}

