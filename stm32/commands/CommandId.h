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

    SET_BACKLIGHT,
    SET_WINDOW,

    /*
     * Graphical commands
     */

    FOREGROUND_COLOUR,
    BACKGROUND_COLOUR,
    RECTANGLE,
    FILL_RECTANGLE,
    CLEAR_RECTANGLE,
    LINE,
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
    ERASE_FLASH_PAGES,
    PROGRAM_FLASH,

    /*
     * Raw bitmap commands
     */

    WRITE_BITMAP,
    WRITE_FLASH_BITMAP,

    /*
     * JPEG bitmap commands
     */

    WRITE_JPEG,
    WRITE_FLASH_JPEG
  };
}
