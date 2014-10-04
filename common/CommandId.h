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

  enum CommandId {

    /*
     * Non-graphical commands
     */

    SET_BACKLIGHT,
    SET_WINDOW,
    SLEEP,
    WAKE,
    GAMMA,

    /*
     * Graphical commands
     */

    FOREGROUND_COLOUR,
    BACKGROUND_COLOUR,
    RECTANGLE,
    FILL_RECTANGLE,
    CLEAR_RECTANGLE,
    CLEAR_SCREEN,
    GRADIENT_FILL_RECTANGLE,
    LINE,
    POLYLINE,
    PLOT,
    ELLIPSE,
    FILL_ELLIPSE,
    BEGIN_WRITING,
    WRITE_DATA,

    /*
     * Text commands
     */

    FONT,
    WRITE_TEXT,
    WRITE_FILLED_TEXT,

    /*
     * Flash commands
     */

    ERASE_FLASH_DEVICE,
    ERASE_FLASH_SECTOR,
    PROGRAM_FLASH,

    /*
     * Raw bitmap commands
     */

    WRITE_BITMAP,
    WRITE_FLASH_BITMAP,
    WRITE_LZG_BITMAP,
    WRITE_FLASH_LZG_BITMAP,

    /*
     * JPEG bitmap commands
     */

    WRITE_JPEG,
    WRITE_FLASH_JPEG,

    /*
     * T1/T2 commands
     */

    TPIN_TIMER_SET_FREQUENCY,
    TPIN_TIMER_INIT_COMPARE,
    TPIN_TIMER_SET_COMPARE,
    TPIN_TIMER_CONTROL,
    TPIN_GPIO_CONFIGURE,
    TPIN_GPIO_CONTROL
  };
}
