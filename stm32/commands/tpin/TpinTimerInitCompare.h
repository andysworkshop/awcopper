/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Initialise the channel's output compare (OC) configuration
   *
   * Parameters:
   *   0       : pin number  : 1 = T1, 2= T2
   *   1,2,3,4 : compare value
   *   5,6     : output compare mode
   *   7,8     : polarity
   *   9,10    : preload
   */

  struct TpinTimerInitCompare {
    static void execute(TPinManager& pinManager,ManagedCircularBuffer& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void TpinTimerInitCompare::execute(TPinManager& pinManager,ManagedCircularBuffer& commandBuffer) {

    uint8_t pinNumber;
    uint32_t compareValue;
    uint16_t params[3];

    // read the parameters

    pinNumber=commandBuffer.managedRead();
    commandBuffer.managedRead(reinterpret_cast<uint8_t *>(&compareValue),4);
    commandBuffer.managedRead(reinterpret_cast<uint8_t *>(params),6);

    // initialise the channel

    pinManager.initCompare(
        static_cast<TPinManager::TPinNumber>(pinNumber),      // pin number
        compareValue,
        params[0],
        params[1],
        params[2]
      );
  }
}
