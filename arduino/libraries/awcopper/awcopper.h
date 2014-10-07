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
   * Timer clockdivision values
   */

  enum ClockDivision {
    CKDIV_1 = 0,
    CKDIV_2 = 0x100,
    CKDIV_4 = 0x200
  };


  /*
   * Timer enumerations
   */

  enum CounterMode {
    COUNTERMODE_UP = 0,
    COUNTERMODE_DOWN = 0x10,
    COUNTERMODE_CENTER1 = 0x20,
    COUNTERMODE_CENTER2 = 0x40,
    COUNTERMODE_CENTER3 = 0x0060
  };

  enum OcMode {
    OCMODE_TIMING = 0,
    OCMODE_ACTIVE = 0X10,
    OCMODE_INACTIVE 0X20,
    OCMODE_TOGGLE = 0X30,
    OCMODE_PWM1 = 0X60,
    OCMODE_PWM2 = 0X70
  };

  enum OcPolarity {
    OCPOLARITY_HIGH = 0,
    OCPOLARITY_LOW = 2
  };

  enum OcPreload {
    OCPRELOAD_ENABLE = 8,
    OCPRELOAD_DISABLE = 0
  };

  enum GpioSlew {
    GPIOSLEW_2 = 0,
    GPIOSLEW_10 = 1,
    GPIOSLEW_50 = 3
  };

  enum GpioDrive {
    GPIODRIVE_PUSHPULL = 0,
    GPIODRIVE_OPENDRAIN = 1,
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

  uint16_t jpeg(const Rectangle& rc,uint32_t count);
  uint16_t bitmap(const Rectangle& rc,uint32_t count);
  uint16_t lzgBitmap(const Rectangle& rc,uint32_t count);

  uint16_t jpegFlash(const Rectangle& rc,uint32_t count,uint32_t address);
  uint16_t bitmapFlash(const Rectangle& rc,uint32_t count,uint32_t address);
  uint16_t lzgBitmapFlash(const Rectangle& rc,uint32_t count,uint32_t address);

  uint16_t t1Frequency(uint32_t period,uint16_t prescaler,ClockDivision clockDivision,CounterMode counterMode);
  uint16_t t2Frequency(uint32_t period,uint16_t prescaler,ClockDivision clockDivision,CounterMode counterMode);

  uint16_t t1InitCompare(uint32_t compareValue,OcMode ocMode,OcPolarity polarity,OcPreload preload);
  uint16_t t2InitCompare(uint32_t compareValue,OcMode ocMode,OcPolarity polarity,OcPreload preload);

  uint16_t t1SetCompare(uint32_t compareValue);
  uint16_t t2SetCompare(uint32_t compareValue);

  uint16_t t1Enable();
  uint16_t t1Disable();
  uint16_t t2Enable();
  uint16_t t2Disable();

  uint16_t t1Gpio(GpioSlew slew,GpioDrive drive);
  uint16_t t2Gpio(GpioSlew slew,GpioDrive drive);

  uint16_t t1GpioSet();
  uint16_t t2GpioSet();
  uint16_t t1GpioReset();
  uint16_t t2GpioReset();
  uint16_t t1GpioControl(bool set);
  uint16_t t2GpioControl(bool set);
}
