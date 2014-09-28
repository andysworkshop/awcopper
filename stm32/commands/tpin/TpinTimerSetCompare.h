/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Set a new output compare value for the channel
   *
   * Parameters:
   *   0       : pin number  : 1 = T1, 2= T2
   *   1,2,3,4 : compare value
   */

  struct TpinTimerSetCompare {

    enum {
      PARAMETER_COUNT=5
    };

    static void execute(TPinManager& pinManager,circular_buffer<uint8_t>& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void TpinTimerSetCompare::execute(TPinManager& pinManager,circular_buffer<uint8_t>& commandBuffer) {

    uint8_t pinNumber;
    uint32_t compareValue;

    // read the parameters

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);

    pinNumber=commandBuffer.read();
    commandBuffer.read(reinterpret_cast<uint8_t *>(&compareValue),4);

    // initialise the channel

    pinManager.setCompare(
        static_cast<TPinManager::TPinNumber>(pinNumber),      // pin number
        compareValue
      );
  }
}
