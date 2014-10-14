/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
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
    SOLID,            // gaps between strokes are filled with the background
    TRANSPARENT       // gaps between strokes are passed over    
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
    OCMODE_ACTIVE = 0x10,
    OCMODE_INACTIVE = 0x20,
    OCMODE_TOGGLE = 0x30,
    OCMODE_PWM1 = 0x60,
    OCMODE_PWM2 = 0x70
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
    GPIOSLEW_2 = 0,                 // 2MHz slew rate
    GPIOSLEW_10 = 1,                // 10MHz slew rate
    GPIOSLEW_50 = 36                // 50MHz slew rate
  };

  enum GpioDrive {
    GPIODRIVE_PUSHPULL = 0,         // normal 2.8V/GND driver
    GPIODRIVE_OPENDRAIN = 1,        // GND driver, open-drain high. must be pulled up externally.
  };


  /*
   * Main coprocessor class. Declare just one of these per application.
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
}


/*
 * Include the inline implementation. Defining everything inline (look it up: many people misunderstand
 * what "inline" actually does) reduces overall code size. In the GraphicsMethods demo it saves ~400 bytes
 * of flash.
 */

#include "awcopperImpl.h"
