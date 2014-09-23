/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Draw an LZG compressed image direct from the Arduino to the display.
   *
   * Parameters:
   *   0,1    : X
   *   2,3    : Y
   *   4,5    : Width
   *   6,7    : Height
   *   8,9,10 : compressed data size
   *   11-n   : compressed data stream
   */

  struct LzgWriter {
    static void execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void LzgWriter::execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer) {

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
    // from the Arduino to the LZG decoder

    CommandReaderInputStream inputStream(commandBuffer,dataSize);
    LzgDecompressionStream decompressor(inputStream,dataSize);

    // draw the graphic

    gl.drawBitmap(rc,decompressor);
  }
}
