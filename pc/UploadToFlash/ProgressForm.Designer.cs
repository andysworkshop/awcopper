namespace UploadToFlash {
  partial class ProgressForm {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing) {
      if(disposing && (components != null)) {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent() {
      this._lblProgress = new System.Windows.Forms.Label();
      this._progressBar = new System.Windows.Forms.ProgressBar();
      this.button1 = new System.Windows.Forms.Button();
      this.SuspendLayout();
      // 
      // _lblProgress
      // 
      this._lblProgress.Location = new System.Drawing.Point(12, 12);
      this._lblProgress.Name = "_lblProgress";
      this._lblProgress.Size = new System.Drawing.Size(348, 19);
      this._lblProgress.TabIndex = 0;
      this._lblProgress.Text = "Progress...";
      // 
      // _progressBar
      // 
      this._progressBar.Location = new System.Drawing.Point(12, 29);
      this._progressBar.Name = "_progressBar";
      this._progressBar.Size = new System.Drawing.Size(348, 20);
      this._progressBar.TabIndex = 1;
      // 
      // button1
      // 
      this.button1.Location = new System.Drawing.Point(138, 65);
      this.button1.Name = "button1";
      this.button1.Size = new System.Drawing.Size(96, 23);
      this.button1.TabIndex = 2;
      this.button1.Text = "Cancel";
      this.button1.UseVisualStyleBackColor = true;
      this.button1.Click += new System.EventHandler(this.button1_Click);
      // 
      // ProgressForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(372, 100);
      this.Controls.Add(this.button1);
      this.Controls.Add(this._progressBar);
      this.Controls.Add(this._lblProgress);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "ProgressForm";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "Programming Images";
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.Label _lblProgress;
    private System.Windows.Forms.ProgressBar _progressBar;
    private System.Windows.Forms.Button button1;
  }
}