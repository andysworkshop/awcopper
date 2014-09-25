/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Draw an image direct from the Arduino to the display. The image may be uncompressed
   * or LZG compressed.
   *
   * Parameters:
   *   0,1    : X
   *   2,3    : Y
   *   4,5    : Width
   *   6,7    : Height
   *   8,9,10 : data size
   *   11-n   : data stream
   */

  struct BitmapWriter {

    enum {
      COMPRESSED,
      UNCOMPRESSED
    };

    static void execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer,int type);
  };


  /*
   * Execute the command
   */

  inline void BitmapWriter::execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer,int type) {

    Panel::LcdPanel& gl(panel.getGraphicsLibrary());
    int16_t params[4];
    uint32_t dataSize;

    // get the rectangle parameters

    while(commandBuffer.availableToRead()<8);
    commandBuffer.read(reinterpret_cast<uint8_t *>(params),8);
    Rectangle rc(params[0],params[1],params[2],params[3]);

    // get the data size

    while(commandBuffer.availableToRead()<3);
    dataSize=0;
    commandBuffer.read(reinterpret_cast<uint8_t *>(&dataSize),3);

    // declare a command reader input stream to manage reading the data direct
    // from the Arduino to the bitmap writer

    CommandReaderInputStream inputStream(commandBuffer,dataSize);

    // draw the graphic

    if(type==COMPRESSED) {
      LzgDecompressionStream decompressor(inputStream,dataSize);
      gl.drawBitmap(rc,decompressor);
    }
    else
      gl.drawBitmap(rc,inputStream);
  }
}
