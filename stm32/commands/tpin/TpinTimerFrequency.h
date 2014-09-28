/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Set the timer frequency of the T1 or T2 pin.
   *
   * Parameters:
   *   0       : pin number  : 1 = T1, 2= T2
   *   1,2,3,4 : period
   *   5,6     : prescaler
   *   7,8     : clock division
   *   9,10    : counter mode
   */

  struct TpinTimerFrequency {

    enum {
      PARAMETER_COUNT=11
    };

    static void execute(TPinManager& pinManager,circular_buffer<uint8_t>& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void TpinTimerFrequency::execute(TPinManager& pinManager,circular_buffer<uint8_t>& commandBuffer) {

    uint8_t pinNumber;
    uint32_t period;
    uint16_t params[3];

    // read the parameters

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);

    pinNumber=commandBuffer.read();
    commandBuffer.read(reinterpret_cast<uint8_t *>(&period),4);
    commandBuffer.read(reinterpret_cast<uint8_t *>(params),6);

    // initialise the pin

    pinManager.setupTimer(
        static_cast<TPinManager::TPinNumber>(pinNumber),      // pin number
        period,         // period
        params[0],      // prescaler
        params[1],      // clock division
        params[2]       // counter mode
      );
  }
}
