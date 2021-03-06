/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Panel wake command
   */

  struct Wake {
    static void execute(Panel& panel);
  };


  /*
   * Execute the command
   */

  inline void Wake::execute(Panel& panel) {
    panel.getGraphicsLibrary().wake();
  }
}
