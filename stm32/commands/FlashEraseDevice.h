/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Erase entire flash device
   */

  struct FlashEraseDevice {
    static void execute() {
      Flash f;
      f.bulkErase();
    }
  };
}
