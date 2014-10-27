using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Threading;
using System.Windows.Forms;

namespace UploadToFlash {
  
  public partial class Main : Form {
  
    private AutoResetEvent _dataReadyEvent=new AutoResetEvent(false);

    /*
     * Constructor
     */

    public Main() {
      InitializeComponent();
    }

    
    /*
     * Main form loaded
     */

    private void Main_Load(object sender,EventArgs e) {

      // fill the serial port combo

      _serialPort.Items.AddRange(SerialPort.GetPortNames());

      if(_serialPort.Items.Count>0) 
        _serialPort.SelectedIndex=_serialPort.Items.Count-1;
      else { 
        MessageBox.Show("No serial ports found, please plug your Arduino in and run the ProgramFlash example sketch","Serial error",MessageBoxButtons.OK,MessageBoxIcon.Error);

        _serialPort.Enabled=false;
        _btnProgram.Enabled=false;
        _btnAdd.Enabled=false;
      }

      EnableControls();
    }


    /*
     * Enable/disable controls
     */

    private void EnableControls() {

      _btnRemove.Enabled=_filesList.SelectedItems.Count>0;
      _btnProgram.Enabled=_filesList.Items.Count>0;
    }


    /*
     * Add files
     */

    private void _btnAdd_Click(object sender,EventArgs e) {

      OpenFileDialog ofd;

      ofd=new OpenFileDialog();
      ofd.Filter="Image Files (*.jpg,*.lzg,*.bin,*.dat)|*.jpg;*.lzg;*.bin;*.dat|All Files (*.*)|*.*||";
      ofd.Multiselect=true;

      if(ofd.ShowDialog()!=DialogResult.OK)
        return;

      foreach(string filename in ofd.FileNames)
        AddFile(filename);
    }


    /*
     * Add a file to the list view
     */

    private void AddFile(string filename) {

      int length,address;
      ListViewItem lvi;

      // add to the list view

      lvi=_filesList.Items.Add(filename);
      lvi.SubItems.Add(_nextAddress.Text);

      // update the next address

      address=int.Parse(_nextAddress.Text);
      length=(int)new FileInfo(filename).Length;
      
      if(length % 256==0)
        address+=length;
      else
        address+=((length/256)+1)*256;

      _nextAddress.Text=address.ToString();

      EnableControls();
    }


    /*
     * Selected item changed
     */

    private void _filesList_SelectedIndexChanged(object sender,EventArgs e) {
      EnableControls();
    }


    /*
     * Remove clicked
     */

    private void _btnRemove_Click(object sender,EventArgs e) {

      foreach(ListViewItem lvi in _filesList.SelectedItems)
        _filesList.Items.Remove(lvi);

      EnableControls();
    }


    /*
     * Edit address
     */

    private void _filesList_DoubleClick(object sender,EventArgs e) {

      if(_filesList.SelectedItems.Count!=1)
        return;

      using(ChangeAddress ca=new ChangeAddress()) {

        ca._address.Text=_filesList.SelectedItems[0].SubItems[1].Text;

        if(ca.ShowDialog(this)!=DialogResult.OK)
          return;

        _filesList.SelectedItems[0].SubItems[1].Text=ca._address.Text;
      }
    }


    /*
     * Do the programming
     */

    private void _btnProgram_Click(object sender,EventArgs e) {
      
      List<ImageFile> files;
      long total;
      ImageFile info;
      SerialPort sp=null;

      // protocol is 8 bits, 1 stop bit

      try {

        sp=new SerialPort(_serialPort.Text,int.Parse(_baudRate.Text),Parity.None,8,StopBits.One);
        sp.ReadTimeout=10000;
        sp.WriteTimeout=10000;
        sp.Open();

        if(!sp.IsOpen) 
          throw new Exception(_serialPort.Text+" is not open");

        sp.DataReceived+=OnDataReady;

        // gather the files

        files=new List<ImageFile>();
        total=0;

        foreach(ListViewItem lvi in _filesList.Items) {
        
          info=new ImageFile() {
            Address=int.Parse(lvi.SubItems[1].Text),
            Info=new FileInfo(lvi.SubItems[0].Text)
          };

          total+=info.Info.Length;

          files.Add(info);
        }

        // set up the progress control

        using(ProgressForm progress=new ProgressForm()) { 
         
          progress.Total=(int)total;

          // program it

          total=0;

          progress.Show();

          foreach(ImageFile i in files) { 

            progress.Prompt=i.Info.Name;
            ProgramFile(sp,i,progress,ref total);
          }
        }
      } catch(Exception ex) {
        MessageBox.Show(ex.Message,"Program Error",MessageBoxButtons.OK,MessageBoxIcon.Exclamation);
      } finally {
        if(sp != null) {
          sp.DataReceived-=OnDataReady;
          sp.Close();
        }
      }
    }


    /*
     * Do the actual programming
     */

    private void ProgramFile(SerialPort sp,ImageFile info,ProgressForm progress,ref long total) {

      int address,pages,i,fileBytesRead,length;
      byte[] page;

      // set up for the programming loop

      address=info.Address;
      length=(int)info.Info.Length;

      page=new byte[256];
      
      if((length % 256)==0)
        pages=length/256;
      else
        pages=(length/256)+1;

      // program each page
      
      using(FileStream fs=info.Info.OpenRead()) {
        
        for(i=0;i<pages;i++) {

          // read the next page - the last one may be a short read if
          // last page and length is not a page size multiple

          Array.Clear(page,0,page.Length);
          fileBytesRead=fs.Read(page,0,page.Length);

          SendPreamble(sp);
          SendAddress(sp,address);
          SendPage(sp,page);
          ReadResponse(sp,progress);

          // update address for next write

          address+=256;

          // update progress
          
          if(progress.Cancelled)
            throw new Exception("User cancelled");
  
          progress.Progress=(int)total;
          total+=fileBytesRead;
        }
      }
    }


    /*
     * Set the data ready event when data arrives
     */

    void OnDataReady(object sender,SerialDataReceivedEventArgs e) {
      _dataReadyEvent.Set();
    }


    /*
     * Send the preamble bytes
     */

    private void SendPreamble(SerialPort sp) {
      
      byte[] preamble=new byte[] { 0x55, 0xaa, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
      sp.Write(preamble,0,preamble.Length);
    }


    /*
     * Send the address
     */

    private void SendAddress(SerialPort sp,int address) {

      // 24-bits, MSB first.

      byte[] buffer=new byte[3];

      buffer[0]=(byte)((address & 0xff0000) >> 16);
      buffer[1]=(byte)((address & 0x00ff00) >> 8);
      buffer[2]=(byte)(address & 0x0000ff);

      sp.Write(buffer,0,buffer.Length);
    }


    /*
     * Send the page of data and its checksum
     */

    private void SendPage(SerialPort sp,byte[] page) {
      
      byte[] pageChecksum=new byte[1];
      int i;

      pageChecksum[0]=0;

      // write page and calc the checksum

      sp.Write(page,0,page.Length);
      for(i=0;i<256;i++)
        pageChecksum[0]^=page[i];

      // write the checksum

      sp.Write(pageChecksum,0,1);
    }


    /*
     * Read the response
     */

    private void ReadResponse(SerialPort sp,ProgressForm progress) {

      byte[] response;
      int i;

      // wait for 30 seconds max

      for(i=0;i<60;i++) {
      
        if(_dataReadyEvent.WaitOne(TimeSpan.FromMilliseconds(500)))
          break;

        Application.DoEvents();

        if(progress.Cancelled)
          throw new Exception("User cancelled");
      }
      
      if(i==60)
        throw new Exception("No response received from the Arduino after 30 seconds");

      response=new byte[1];
      sp.Read(response,0,1);

      if(response[0]!=1)
        throw new Exception("Serial transfer error: the Arduino NACK'd the data");
    }
  }
}
