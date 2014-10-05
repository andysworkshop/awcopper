/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Set the foreground/background colour for graphics operations
   *
   * Parameters:
   *   0,1,2 : RR,GG,BB colour
   */

  struct Colour {

    enum {
      PARAMETER_COUNT = 3
    };

    static void execute(Panel& panel,ManagedCircularBuffer& commandBuffer,bool fg);
  };


  /*
   * Execute the command
   */

  inline void Colour::execute(Panel& panel,ManagedCircularBuffer& commandBuffer,bool fg) {

    Panel::LcdPanel& gl(panel.getGraphicsLibrary());
    uint8_t parameters[3];

    // wait for, and then read the parameters

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);
    commandBuffer.managedRead(reinterpret_cast<uint8_t *>(parameters),PARAMETER_COUNT);

    // set the colour

    if(fg)
      gl.setForeground(parameters[0],parameters[1],parameters[2]);
    else
      gl.setBackground(parameters[0],parameters[1],parameters[2]);
  }
}
