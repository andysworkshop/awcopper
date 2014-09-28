/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * enable or disable timer output
   *
   * Parameters:
   *   0 : pin number  : 1 = T1, 2= T2
   *   1 : 0 = off, 1 = on
   */

  struct TpinTimerControl {

    enum {
      PARAMETER_COUNT=2
    };

    static void execute(TPinManager& pinManager,circular_buffer<uint8_t>& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void TpinTimerControl::execute(TPinManager& pinManager,circular_buffer<uint8_t>& commandBuffer) {

    uint8_t params[2];

    // read the parameters

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);
    commandBuffer.read(params,PARAMETER_COUNT);

    // initialise the channel

    pinManager.controlTimer(
        static_cast<TPinManager::TPinNumber>(params[0]),      // pin number
        !!params[1]
      );
  }
}

