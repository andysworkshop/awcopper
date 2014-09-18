/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Issue the begin writing command that prepares for streaming data
   */

  struct BeginWriting {
    static void execute(Panel& panel);
  };


  /*
   * Execute the command
   */

  inline void BeginWriting::execute(Panel& panel) {
    panel.getGraphicsLibrary().beginWriting();
  }
}
