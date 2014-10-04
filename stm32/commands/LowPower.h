/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Sleep / wakeup the panel
   */

  struct LowPower {

    /*
     * Operations
     */

    enum {
      SLEEP,
      WAKE
    };

    static void execute(Panel& panel,int mode);
  };


  /*
   * Execute the command
   */

  inline void LowPower::execute(Panel& panel,int mode) {

    if(mode==SLEEP)
      panel.getGraphicsLibrary().sleep();
    else
      panel.getGraphicsLibrary().wake();
  }
}
