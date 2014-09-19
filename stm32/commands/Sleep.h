/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Panel sleep command
   */

  struct Sleep {
    static void execute(Panel& panel);
  };


  /*
   * Execute the command
   */

  inline void Sleep::execute(Panel& panel) {
    panel.getGraphicsLibrary().sleep();
  }
}
