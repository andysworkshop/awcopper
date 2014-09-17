/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Set the on-screen window to which raw graphic operations will be constrained. This is
   * mainly only used in conjunction with the raw BEGIN_WRITING/WRITE_DATA commands
   *
   * Parameters:
   *   0,1 Window top-left X
   *   2,3 Window top-left Y
   *   4,5 Window bottom-right X
   *   6,7 Window bottom-right Y
   */

  struct SetWindow {

    enum {
      PARAMETER_COUNT = 8
    };

    static void execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void SetWindow::execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer) {

    Panel::LcdPanel& gl(panel.getGraphicsLibrary());
    int16_t parameters[4];

    // wait for, and then read the parameters

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);
    commandBuffer.read(reinterpret_cast<uint8_t *>(parameters),PARAMETER_COUNT);

    // set the display window

    gl.moveTo(parameters[0],parameters[1],parameters[2],parameters[3]);
  }
}
