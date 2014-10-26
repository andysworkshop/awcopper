using System;
using System.Windows.Forms;

namespace UploadToFlash {
  public partial class ChangeAddress : Form {
    public ChangeAddress() {
      InitializeComponent();
    }

    private void _address_TextChanged(object sender,EventArgs e) {
      EnableControls();
    }

    private void ChangeAddress_Load(object sender,EventArgs e) {
      EnableControls();
    }

    private void EnableControls() {
      _btnOK.Enabled=_address.TextLength>0;
    }
  }
}
