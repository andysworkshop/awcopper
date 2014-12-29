/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace awc {

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
