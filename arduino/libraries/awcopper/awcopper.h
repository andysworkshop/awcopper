/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once

#include "colournames.h"


namespace awc {

  #include "CommandId.h"

  /*
   * Main coprocessor class.
   */

  class CoProcessor {

    public:
      
      /*
       * The address of the STM32 slave on the I2C bus
       */

      enum {
        SLAVE_ADDRESS = 0x38
      };

      static uint8_t buffer[32];

    public:
      void begin();
      void reset();

      CoProcessor& operator<<(uint16_t count);
  };


  /*
   * Command declarations
   */

  uint16_t backlight(uint8_t percentage);
  uint16_t background(uint32_t colorref);
  uint16_t foreground(uint32_t colorref);
  uint16_t clearRectangle(uint16_t x,uint16_t y,uint16_t width,uint16_t height);
  uint16_t fillRectangle(uint16_t x,uint16_t y,uint16_t width,uint16_t height);
  uint16_t rectangle(uint16_t x,uint16_t y,uint16_t width,uint16_t height);
  uint16_t line(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
  uint16_t reset();
}
