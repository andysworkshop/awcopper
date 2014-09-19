/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Draw single line
   *
   * Parameters:
   *   0,1 : X1
   *   2,3 : Y1
   *   4,5 : X1
   *   6,7 : Y1
   */

  struct Line {

    enum {
      PARAMETER_COUNT = 8
    };

    static void execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void Line::execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer) {

    Panel::LcdPanel& gl(panel.getGraphicsLibrary());
    int16_t params[4];

    // read the co-ordinates and draw the line

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);
    commandBuffer.read(reinterpret_cast<uint8_t *>(params),PARAMETER_COUNT);

    gl.drawLine(Point(params[0],params[1]),Point(params[2],params[3]));
  }
}
