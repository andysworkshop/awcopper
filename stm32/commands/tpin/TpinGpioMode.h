/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Set the GPIO mode of the T1 or T2 pin.
   *
   * Parameters:
   *   0 : pin number  : 1 = T1, 2= T2
   *   1 : slew rate   : 0 = 2MHz, 1 = 10MHz, 3 = 50MHz
   *   2 : output type : 0 = push-pull, 1 = open-drain
   */

  struct TpinGpioMode {

    enum {
      PARAMETER_COUNT = 3,
    };

    static void execute(TPinManager& pinManager,ManagedCircularBuffer& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void TpinGpioMode::execute(TPinManager& pinManager,ManagedCircularBuffer& commandBuffer) {

    uint8_t params[3];

    // read the parameters

    commandBuffer.managedRead(params,PARAMETER_COUNT);

    // initialise the pin

    pinManager.setupGpio(
        static_cast<TPinManager::TPinNumber>(params[0]),      // pin number
        static_cast<GPIOSpeed_TypeDef>(params[1]),            // slew rate
        static_cast<Gpio::GpioOutputType>(params[2])          // output type
      );
  }
}
