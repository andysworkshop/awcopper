/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#include <Wire.h>
#include <stdint.h>
#include "awcopper.h"


namespace awc {

  /*
   * Static class member initialisation
   */

  uint8_t CoProcessor::buffer[32];


  /*
   * Initialise the wire library
   */

  void CoProcessor::begin() {
    ::Wire.begin();
  }


  /*
   * Write out the number of bytes already prepared in the static buffer
   */

  CoProcessor& CoProcessor::operator<<(uint16_t count) {

    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(CoProcessor::buffer,count);
    Wire.endTransmission();

    return *this;
  }


  /*
   * Set the backlight to the given percentage level
   */

  uint16_t backlight(uint8_t percentage) {
    
    uint8_t *ptr=CoProcessor::buffer;

    *ptr++=CoProcessor::SET_BACKLIGHT;
    *ptr=percentage;

    return 2;
  }
}
