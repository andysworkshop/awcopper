/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Set a gamma curve
   *
   * Parameters:
   *   0..12 : 13 byte gamma curve values (see R61523Gamma)
   */

  struct Gamma {

    enum {
      PARAMETER_COUNT = 13
    };

    static void execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void Gamma::execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer) {

    Panel::LcdPanel& gl(panel.getGraphicsLibrary());
    uint8_t params[13];

    // read the parameters and set the gamma

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);
    commandBuffer.read(reinterpret_cast<uint8_t *>(params),PARAMETER_COUNT);

    gl.applyGamma(R61523Gamma(params));
  }
}
