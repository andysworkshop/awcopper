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
#include "Bytes.h"
#include "WireStream.h"


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
   * Gradient fill directions (the numbers are important)
   */

  enum Direction {
    HORIZONTAL = 0,
    VERTICAL = 1
  };


  /*
   * Main coprocessor class.
   */

  class CoProcessor {

    protected:
      WireStream byteStream;    // used to stream out data for the long byte-stream commands (program, bitmaps etc)

    public:
      
      /*
       * The address of the STM32 slave on the I2C bus
       */

      enum {
        SLAVE_ADDRESS = 0x38
      };

      /*
       * Commands get buffered here pending transmission. BUFFER_LENGTH is an exceedingly 
       * poorly named macro that you'll find in <Wire.h>.
       */

      static uint8_t buffer[BUFFER_LENGTH];

      /*
       * Bytes remaining to be streamed in for the last command
       */

      static uint32_t bytesRemaining;

    public:
      void reset();

      CoProcessor& operator<<(uint16_t count);        // command buffer streaming
      CoProcessor& operator<<(const Bytes& bytes);    // variable bytes data streaming
  };


  /*
   * Traits classes for portrait and landscape
   */

  struct LandscapeCoProcessor : CoProcessor {
    enum {
      WIDTH = 640,
      HEIGHT = 360
    };
  };

  struct PortraitCoProcessor : CoProcessor {
    enum {
      WIDTH = 360,
      HEIGHT = 640
    };
  };


  /*
   * Command declarations
   */

  uint16_t backlight(uint8_t percentage);
  uint16_t window(const Rectangle& rc);
  uint16_t sleep();
  uint16_t wake();
  uint16_t gamma(const Gamma& gamma);
  uint16_t beginWriting();
  uint16_t writeData(const void *data,uint16_t count);

  uint16_t background(uint32_t colorref);
  uint16_t foreground(uint32_t colorref);

  uint16_t clear();
  uint16_t clearRectangle(const Rectangle& rc);
  uint16_t fillRectangle(const Rectangle& rc);
  uint16_t rectangle(const Rectangle& rc);
  uint16_t gradientFillRectangle(const Rectangle& rc,Direction dir,uint32_t firstColour,uint32_t lastColour);

  uint16_t line(const Point& p1,const Point& p2);
  uint16_t polyline(const Point *p,uint8_t count);

  uint16_t font(FontId fid);
  uint16_t text(const Point& p,const char *str,TextMode textMode=TRANSPARENT);

  uint16_t plot(const Point& p);

  uint16_t ellipse(const Point& center,const Size& size);
  uint16_t fillEllipse(const Point& center,const Size& size);

  uint16_t eraseFlash();
  uint16_t erase4KSector(uint32_t address);
  uint16_t erase8KSector(uint32_t address);
  uint16_t erase64KSector(uint32_t address);

  uint16_t program(uint32_t address);
}
