/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Draw a single pixel outline of a rectangle in the foreground colour
   *
   * Parameters:
   *   0,1 : center X
   *   2,3 : center Y
   *   4,5 : width
   *   6,7 : height
   */

  struct EllipseOps {

    enum {
      PARAMETER_COUNT = 8,

      FILLED,
      OUTLINED
    };

    static void execute(Panel& panel,ManagedCircularBuffer& commandBuffer,int operType);
  };


  /*
   * Execute the command
   */

  inline void EllipseOps::execute(Panel& panel,ManagedCircularBuffer& commandBuffer,int operType) {

    Panel::LcdPanel& gl(panel.getGraphicsLibrary());
    int16_t parameters[4];

    // wait for, and then read the parameters

    commandBuffer.managedRead(reinterpret_cast<uint8_t *>(parameters),PARAMETER_COUNT);

    // draw the ellipse

    Point center(parameters[0],parameters[1]);
    Size size(parameters[2],parameters[3]);

    switch(operType) {

      case FILLED:
        gl.fillEllipse(center,size);
        break;

      default:
        gl.drawEllipse(center,size);
        break;
    }
  }
}
