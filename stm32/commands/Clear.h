/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Convenience function to clear the screen to the background colour.
   */

  struct Clear {
    static void execute(Panel& panel);
  };


  /*
   * Execute the command
   */

  inline void Clear::execute(Panel& panel) {
    panel.getGraphicsLibrary().clearScreen();
  }
}
