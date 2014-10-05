/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Fill a rectangle with a gradient
   *
   * Parameters:
   *   0,1      : top-left X
   *   2,3      : top-left Y
   *   4,5      : width
   *   6,7      : height
   *   8        : direction (0=horizontal, 1=vertical)
   *   9,10,11  : rr,gg,bb first colour
   *   12,13,14 : rr,gg,bb last colour
   */

  struct GradientRect {
    static void execute(Panel& panel,ManagedCircularBuffer& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void GradientRect::execute(Panel& panel,ManagedCircularBuffer& commandBuffer) {

    Panel::LcdPanel& gl(panel.getGraphicsLibrary());
    int16_t rparams[4];
    uint8_t dir,cparams[6];
    Panel::LcdPanel::tCOLOUR first,last;

    // read the rect

    while(commandBuffer.availableToRead()<8);
    commandBuffer.managedRead(reinterpret_cast<uint8_t *>(rparams),8);
    Rectangle rc(rparams[0],rparams[1],rparams[2],rparams[3]);

    // read the direction

    while(commandBuffer.availableToRead()<1);
    dir=commandBuffer.managedRead();

    // read the colours

    while(commandBuffer.availableToRead()<6);
    commandBuffer.managedRead(cparams,6);

    first=((uint32_t)cparams[0]) << 16 |
          ((uint32_t)cparams[1]) << 8 |
          cparams[2];

    last=((uint32_t)cparams[3]) << 16 |
         ((uint32_t)cparams[4]) << 8 |
         cparams[5];

    // do it

    gl.gradientFillRectangle(rc,dir==0 ? HORIZONTAL : VERTICAL,first,last);
  }
}
