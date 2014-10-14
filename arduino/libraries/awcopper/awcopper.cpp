/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#include "awcopper.h"


namespace awc {

  /*
   * Static class member initialisation
   */

  uint8_t CoProcessor::buffer[BUFFER_LENGTH];
  uint32_t CoProcessor::bytesRemaining=0;
}
