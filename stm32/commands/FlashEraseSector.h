/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Erase a flash sector of either 4K, 8K or 64K
   *
   * Parameters:
   *   0     : The sector size (4 = 4Kb, 8 = 8Kb, 64 = 64Kb)
   *   1,2,3 : The 24-bit address of the sector
   */

  struct FlashEraseSector {

    enum {
      PARAMETER_COUNT = 4
    };

    static void execute(circular_buffer<uint8_t>& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void FlashEraseSector::execute(circular_buffer<uint8_t>& commandBuffer) {

    uint8_t sectorSize;
    uint32_t address;
    Flash f;

    // wait for the params to be available then read

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);

    sectorSize=commandBuffer.read();
    address=0;
    commandBuffer.read(reinterpret_cast<uint8_t *>(&address),3);

    // do the erase

    f.eraseSector(
        address,
        sectorSize==4 ? Flash::SECTOR_4K :
            sectorSize==8 ? Flash::SECTOR_8K : Flash::SECTOR_64K);
  }
}
