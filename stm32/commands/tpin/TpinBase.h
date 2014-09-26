/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Base structure for timer pin functionality
   */

  struct TPinBase {

    /*
     * Timer pin numbers
     */

    enum class Tpin {
      T1 = 1,           // pin T1
      T2 = 2            // pin T2
    };
  };
}
