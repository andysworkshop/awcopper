/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Set the backlight.
   * Parameters:
   *   0 : the backlight percentage, 0..100
   */

  struct SetBacklight {

    enum {
      PARAMETER_COUNT = 1
    };

    static void execute(Panel& panel,ManagedCircularBuffer& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void SetBacklight::execute(Panel& panel,ManagedCircularBuffer& commandBuffer) {

    uint8_t percentage;

    // wait for, and then read the parameter

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);
    percentage=commandBuffer.managedRead();

    // set the percentage

    panel.getBacklight().setPercentage(percentage);
  }
}
