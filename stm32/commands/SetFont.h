/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Set the currently selected font for text operations
   *
   * Parameters:
   *   0 : the font index
   */

  struct SetFont {
    static void execute(Panel& panel,ManagedCircularBuffer& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void SetFont::execute(Panel& panel,ManagedCircularBuffer& commandBuffer) {

    uint8_t font;
    Panel::LcdPanel& gl(panel.getGraphicsLibrary());

    // read the font index

    font=commandBuffer.managedRead();

    // set the selected font

    gl << panel.getFontManager().getFont(font);
  }
}
