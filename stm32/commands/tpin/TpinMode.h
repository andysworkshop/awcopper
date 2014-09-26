/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Set the mode of the T1 or T2 pin.
   *
   * Parameters:
   *   0 : 1 = T1, 2= T2
   *   1 : 0 = GPIO, 1 = Timer
   */

  struct TpinMode : TPinBase {

    enum {
      PARAMETER_COUNT = 2,
    };

    /*
     * Possible pin modes (GPIO or TIMER)
     */

    enum class Mode {
      GPIO = 1,
      TIMER = 2
    };

    static void execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void Plot::execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer) {

    Panel::LcdPanel& gl(panel.getGraphicsLibrary());
    int16_t params[2];

    // read the co-ordinates and plot the point

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);
    commandBuffer.read(reinterpret_cast<uint8_t *>(params),PARAMETER_COUNT);
    gl.plotPoint(Point(params[0],params[1]));
  }
}
