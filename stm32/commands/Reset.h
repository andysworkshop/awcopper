/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Reset the MCU
   */

  struct Reset {

    static void execute() {
      NVIC_SystemReset();
    }
  };
}
