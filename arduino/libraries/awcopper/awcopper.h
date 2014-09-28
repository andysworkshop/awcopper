/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace awc {

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
       * All command identifiers
       */

      enum CommandId {

        /*
         * Non-graphical commands
         */

        SET_BACKLIGHT = 0,
        SET_WINDOW = 1,
        SLEEP = 2,
        WAKE = 3,
        GAMMA = 4,

        /*
         * Graphical commands
         */

        FOREGROUND_COLOUR = 5,
        BACKGROUND_COLOUR = 6,
        RECTANGLE = 7,
        FILL_RECTANGLE = 8,
        CLEAR_RECTANGLE = 9,
        LINE = 10,
        POLYLINE = 11,
        PLOT = 12,
        ELLIPSE = 13,
        FILL_ELLIPSE = 14,
        BEGIN_WRITING = 15,
        WRITE_DATA = 16,

        /*
         * Text commands
         */

        FONT = 17,
        WRITE_TEXT = 18,
        WRITE_FILLED_TEXT = 19,

        /*
         * Flash commands
         */

        ERASE_FLASH_DEVICE = 20,
        ERASE_FLASH_SECTOR = 21,
        PROGRAM_FLASH = 22,

        /*
         * Raw bitmap commands
         */

        WRITE_BITMAP = 23,
        WRITE_FLASH_BITMAP = 24,
        WRITE_LZG_BITMAP = 25,
        WRITE_FLASH_LZG_BITMAP = 26,

        /*
         * JPEG bitmap commands
         */

        WRITE_JPEG = 27,
        WRITE_FLASH_JPEG = 28,

        /*
         * T1/T2 commands
         */

        TPIN_TIMER_SET_FREQUENCY = 29,
        TPIN_TIMER_INIT_COMPARE = 30,
        TPIN_TIMER_SET_COMPARE = 31,
        TPIN_TIMER_CONTROL = 32,
        TPIN_GPIO_CONFIGURE = 33,
        TPIN_GPIO_CONTROL = 34
      };

      static uint8_t buffer[32];

    public:
      void begin();

      CoProcessor& operator<<(uint16_t count);
  };


  /*
   * Command declarations
   */

  uint16_t backlight(uint8_t percentage);
}
