using System.IO;

namespace UploadToFlash {

  /*
   * Holder for file + address
   */

  class ImageFile {
    public FileInfo Info {  get; set; }
    public int Address {  get; set; }
  }
}
