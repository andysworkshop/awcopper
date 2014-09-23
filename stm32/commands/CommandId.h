/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Command identifiers
   */

  enum class CommandId {

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

    /*
     * Raw LZG compressed bitmap commands
     */

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

    TIMER_SET_FREQUENCY = 29,
    TIMER_SET_DUTY_CYCLE = 30,
    TIMER_ON_OFF = 31
  };
}
