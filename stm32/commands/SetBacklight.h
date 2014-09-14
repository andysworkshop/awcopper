/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Set the backlight
   */

  struct SetBacklight : CommandBase {

    enum {
      PARAMETER_COUNT = 1
    };

    virtual ~SetBacklight() {}
    virtual void execute(const uint8_t *parameters) override;
  };
}
