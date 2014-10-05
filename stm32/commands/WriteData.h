/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Write N pixels of data directly to the panel. The BeginWriting command must have
   * been issued before this one.
   *
   * Parameters
   *   0,1 :  Number of pixels to write
   *   [ 2,3, Pixel 0
   *     4,5  Pixel 1 ...
   *   ]
   */

  struct WriteData {

    enum {
      PIXEL_BUFFER = 500      // max number of pixels to read before transfer to LCD
    };

    static void execute(Panel& panel,ManagedCircularBuffer& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void WriteData::execute(Panel& panel,ManagedCircularBuffer& commandBuffer) {

    uint16_t count,nPixels,buffer[PIXEL_BUFFER];
    Panel::LcdPanel& gl(panel.getGraphicsLibrary());

    // wait for the number of pixels to arrive

    while(commandBuffer.availableToRead()<2);
    commandBuffer.managedRead(reinterpret_cast<uint8_t *>(&nPixels),2);

    while(nPixels) {

      // wait for pixel bytes to arrive

      if(nPixels<=PIXEL_BUFFER)
        count=nPixels;
      else
        count=PIXEL_BUFFER;

      while(commandBuffer.availableToRead()<count*2);   // 2 bytes per pixel

      // transfer to LCD

      gl.rawTransfer(buffer,count);

      // decrease remaining

      nPixels-=count;
    }
  }
}
