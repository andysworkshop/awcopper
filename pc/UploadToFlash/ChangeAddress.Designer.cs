namespace UploadToFlash {
  partial class ChangeAddress {
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
      this.label1 = new System.Windows.Forms.Label();
      this._address = new System.Windows.Forms.TextBox();
      this._btnOK = new System.Windows.Forms.Button();
      this._btnCancel = new System.Windows.Forms.Button();
      this.SuspendLayout();
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(11, 15);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(84, 13);
      this.label1.TabIndex = 1;
      this.label1.Text = "Address in flash:";
      // 
      // _address
      // 
      this._address.Location = new System.Drawing.Point(101, 12);
      this._address.Name = "_address";
      this._address.Size = new System.Drawing.Size(85, 20);
      this._address.TabIndex = 2;
      this._address.Text = "0";
      this._address.TextChanged += new System.EventHandler(this._address_TextChanged);
      // 
      // _btnOK
      // 
      this._btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
      this._btnOK.Location = new System.Drawing.Point(19, 43);
      this._btnOK.Name = "_btnOK";
      this._btnOK.Size = new System.Drawing.Size(75, 23);
      this._btnOK.TabIndex = 3;
      this._btnOK.Text = "OK";
      this._btnOK.UseVisualStyleBackColor = true;
      // 
      // _btnCancel
      // 
      this._btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
      this._btnCancel.Location = new System.Drawing.Point(100, 43);
      this._btnCancel.Name = "_btnCancel";
      this._btnCancel.Size = new System.Drawing.Size(75, 23);
      this._btnCancel.TabIndex = 4;
      this._btnCancel.Text = "Cancel";
      this._btnCancel.UseVisualStyleBackColor = true;
      // 
      // ChangeAddress
      // 
      this.AcceptButton = this._btnOK;
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.CancelButton = this._btnCancel;
      this.ClientSize = new System.Drawing.Size(198, 78);
      this.Controls.Add(this._btnCancel);
      this.Controls.Add(this._btnOK);
      this.Controls.Add(this._address);
      this.Controls.Add(this.label1);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "ChangeAddress";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "ChangeAddress";
      this.Load += new System.EventHandler(this.ChangeAddress_Load);
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.Button _btnOK;
    private System.Windows.Forms.Button _btnCancel;
    public System.Windows.Forms.TextBox _address;
  }
}