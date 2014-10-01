/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once

#include "ColourNames.h"
#include "Size.h"
#include "Point.h"
#include "Rectangle.h"
#include "Gamma.h"


namespace awc {

  #include "CommandId.h"


  /*
   * Font identifiers - order must match the STM32 FontManager initialisation
   */
   
  enum FontId {
    APPLE,
    ATARI,
    DOS,
    KYROU_BOLD,
    KYROU,
    NINTENDO,
    PIXELADE,
    PROGGY,
    GOLDFISH        
  };


  /*
   * Text output modes
   */

   enum TextMode {
     SOLID,
     TRANSPARENT
   };


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

      /*
       * Commands get buffered here pending transmission. BUFFER_LENGTH is an extremely 
       * poorly named macro that you'll find in <Wire.h>.
       */

      static uint8_t buffer[BUFFER_LENGTH];

    public:
      void begin();
      void reset();

      CoProcessor& operator<<(uint16_t count);
  };

  /*
   * Command declarations
   */

  uint16_t backlight(uint8_t percentage);
  uint16_t window(const Rectangle& rc);
  uint16_t sleep();
  uint16_t wake();
  uint16_t gamma(const Gamma& gamma);

  uint16_t background(uint32_t colorref);
  uint16_t foreground(uint32_t colorref);

  uint16_t clearRectangle(const Rectangle& rc);
  uint16_t fillRectangle(const Rectangle& rc);
  uint16_t rectangle(const Rectangle& rc);

  uint16_t line(const Point& p1,const Point& p2);
  uint16_t polyline(const Point *p,uint8_t count);

  uint16_t font(FontId fid);
  uint16_t text(const Point& p,const char *str,TextMode textMode=TRANSPARENT);
}
