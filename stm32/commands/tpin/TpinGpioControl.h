/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * set the GPIO pin level
   *
   * Parameters:
   *   0 : pin number  : 1 = T1, 2= T2
   *   1 : 0 = LOW, 1 = HIGH
   */

  struct TpinGpioControl {

    enum {
      PARAMETER_COUNT=2
    };

    static void execute(TPinManager& pinManager,ManagedCircularBuffer& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void TpinGpioControl::execute(TPinManager& pinManager,ManagedCircularBuffer& commandBuffer) {

    uint8_t params[2];

    // read the parameters

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);
    commandBuffer.managedRead(params,PARAMETER_COUNT);

    // initialise the channel

    pinManager.controlGpio(
        static_cast<TPinManager::TPinNumber>(params[0]),      // pin number
        !!params[1]
      );
  }
}
