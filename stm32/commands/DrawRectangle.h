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
   *   0,1 : top-left X
   *   2,3 : top-left Y
   *   4,5 : width
   *   6,7 : height
   */

  struct DrawRectangle {

    enum {
      PARAMETER_COUNT = 8
    };

    static void execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void DrawRectangle::execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer) {

    Panel::LcdPanel& gl(panel.getGraphicsLibrary());
    int16_t parameters[4];

    // wait for, and then read the parameters

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);
    commandBuffer.read(reinterpret_cast<uint8_t *>(parameters),PARAMETER_COUNT);

    // draw the rectangle

    gl.drawRectangle(Rectangle(parameters[0],parameters[1],parameters[2],parameters[3]));
  }
}
