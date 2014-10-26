/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Rectangle operations (outlined, filled, cleared)
   *
   * Parameters:
   *   0,1 : top-left X
   *   2,3 : top-left Y
   *   4,5 : width
   *   6,7 : height
   */

  struct RectangleOps {

    enum {
      PARAMETER_COUNT = 8,

      FILLED,
      CLEARED,
      OUTLINED
    };

    static void execute(Panel& panel,ManagedCircularBuffer& commandBuffer,int operType);
  };


  /*
   * Execute the command
   */

  inline void RectangleOps::execute(Panel& panel,ManagedCircularBuffer& commandBuffer,int operType) {

    Panel::LcdPanel& gl(panel.getGraphicsLibrary());
    int16_t parameters[4];

    // wait for, and then read the parameters

    commandBuffer.managedRead(reinterpret_cast<uint8_t *>(parameters),PARAMETER_COUNT);

    // draw the rectangle

    Rectangle rc(parameters[0],parameters[1],parameters[2],parameters[3]);

    switch(operType) {

      case CLEARED:
        gl.clearRectangle(rc);
        break;

      case FILLED:
        gl.fillRectangle(rc);
        break;

      default:
        gl.drawRectangle(rc);
        break;
    }
  }
}
