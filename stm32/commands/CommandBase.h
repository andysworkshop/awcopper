/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Base class for all commands
   */

  struct CommandBase {

    /*
     * Zero-based enumeration of all available commands
     */

    enum {
      ID_SET_BACKLIGHT = 0
    };

    virtual ~CommandBase() {}
    virtual void execute(const uint8_t *parameters)=0;
  };
}
