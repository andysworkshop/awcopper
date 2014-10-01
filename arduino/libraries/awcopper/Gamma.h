/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace awc {

  /*
   * Gamma values for the R61523. This class is designed to be bit-copiable
   */

  struct Gamma {

    uint8_t _gamma[13];

    /*
     * Constructor
     */

    Gamma(const uint8_t *levels) {
      memcpy(_gamma,levels,13);
    }

    /*
     * Access operator
     */

    uint8_t operator[](int pos) const {
      return _gamma[pos];
    }


    /*
     * Create a default set of levels
     */

    static Gamma getDefault() {
      const uint8_t levels[13]={ 0xe,0,1,1,0,0,0,0,0,0,3,4,0 };
      return Gamma(levels);
    }
  };
}
