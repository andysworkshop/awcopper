/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Program a flash 256 byte page
   *
   * Parameters:
   *   0,1,2 : The 24-bit address of the page
   *   3-258 : The page of data
   */

  struct FlashProgram {

    enum {
      PARAMETER_COUNT = 259
    };

    static void execute(circular_buffer<uint8_t>& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void FlashProgram::execute(circular_buffer<uint8_t>& commandBuffer) {

    uint32_t address;
    uint8_t page[256];
    Flash f;

    // wait for the params to be available then read

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);

    // get the address first

    address=0;
    commandBuffer.read(reinterpret_cast<uint8_t *>(&address),3);

    // get the page next

    commandBuffer.read(page,256);

    // do the

    f.writePage(address,page);
  }
}
