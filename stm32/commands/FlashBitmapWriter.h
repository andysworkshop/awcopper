/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Draw an image sourced from flash to the display. The image may be uncompressed
   * or LZG compressed.
   *
   * Parameters:
   *   0,1      : X
   *   2,3      : Y
   *   4,5      : Width
   *   6,7      : Height
   *   8,9,10   : data size
   *   11,12,13 : source address in flash
   */

  struct FlashBitmapWriter {

    enum class Storage {
      COMPRESSED,
      UNCOMPRESSED,
      JPEG
    };

    static void execute(Panel& panel,ManagedCircularBuffer& commandBuffer,Storage type);
  };


  /*
   * Execute the command
   */

  inline void FlashBitmapWriter::execute(Panel& panel,ManagedCircularBuffer& commandBuffer,Storage type) {

    int16_t params[4];
    uint32_t dataSize,flashAddress;
    Panel::LcdPanel& gl(panel.getGraphicsLibrary());

    // get the rectangle parameters

    while(commandBuffer.availableToRead()<8);
    commandBuffer.managedRead(reinterpret_cast<uint8_t *>(params),8);
    Rectangle rc(params[0],params[1],params[2],params[3]);

    // get the data size

    while(commandBuffer.availableToRead()<3);
    dataSize=0;
    commandBuffer.managedRead(reinterpret_cast<uint8_t *>(&dataSize),3);

    // get the flash address

    while(commandBuffer.availableToRead()<3);
    flashAddress=0;
    commandBuffer.managedRead(reinterpret_cast<uint8_t *>(&flashAddress),3);

    // set up the drawing rectangle and get ready for receiving data

    gl.moveTo(rc);
    gl.beginWriting();

    // need a flash input stream to get the data

    FlashInputStream inputStream(flashAddress,dataSize);

    // draw the graphic

    if(type==Storage::COMPRESSED) {
      LzgDecompressionStream decompressor(inputStream,dataSize);
      gl.drawBitmap(rc,decompressor);
    }
    else if(type==Storage::UNCOMPRESSED)
      gl.drawBitmap(rc,inputStream);
    else if(type==Storage::JPEG)
      gl.drawJpeg(rc,inputStream);
  }
}
