using System.Windows.Forms;

namespace UploadToFlash {
  
  public partial class ProgressForm : Form {
    
    private bool _cancelled;

    public ProgressForm() {
      InitializeComponent();
      _cancelled=false;
    }

    public int Total {
      set {
        _progressBar.Maximum=value;
      }
    }


    public int Progress {
      set {
        _progressBar.Value=value;
        Application.DoEvents();
      }
    }

    public string Prompt {
      set {
        _lblProgress.Text=value;
        Application.DoEvents();
      }
    }

    private void button1_Click(object sender,System.EventArgs e) {
      _cancelled=true;
    }

    public bool Cancelled { get { return _cancelled; } }
  }
}
