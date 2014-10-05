/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Plot a single point
   *
   * Parameters:
   *   0,1 : X1
   *   2,3 : Y1
   */

  struct Plot {

    enum {
      PARAMETER_COUNT = 4
    };

    static void execute(Panel& panel,ManagedCircularBuffer& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void Plot::execute(Panel& panel,ManagedCircularBuffer& commandBuffer) {

    Panel::LcdPanel& gl(panel.getGraphicsLibrary());
    int16_t params[2];

    // read the co-ordinates and plot the point

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);
    commandBuffer.managedRead(reinterpret_cast<uint8_t *>(params),PARAMETER_COUNT);
    gl.plotPoint(Point(params[0],params[1]));
  }
}
