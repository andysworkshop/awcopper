/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


/**
 * Font manager class
 */

class FontManager {

  protected:

    enum {
      NUM_FONTS = 9
    };

    const Font *_fonts[NUM_FONTS];

  public:
    FontManager();

    const Font& getFont(uint8_t index) const;
};


/*
 * Constructor
 */

inline FontManager::FontManager() {

  _fonts[0]=new Font_APPLE8;

// there's not enough space in flash in the debug build to hold all these fonts

#if !defined(DEBUG)
  _fonts[1]=new Font_ATARIST8X16SYSTEMFONT16;
  _fonts[2]=new Font_PERFECT_DOS_VGA_437_WIN16;
  _fonts[3]=new Font_KYROU_9_REGULAR_BOLD8;
  _fonts[4]=new Font_KYROU_9_REGULAR8;
  _fonts[5]=new Font_NINTENDO_DS_BIOS16;
  _fonts[6]=new Font_PIXELADE13;
  _fonts[7]=new Font_PROGGYCLEAN16;
  _fonts[8]=new Font_VOLTER__28GOLDFISH_299;
#endif
}


/*
 * Get the font reference at a position
 */

inline const Font& FontManager::getFont(uint8_t index) const {

#if defined(DEBUG)
  return *_fonts[index==0 ? index : 0];   // keeps the compiler quiet
#else
  return *_fonts[index];
#endif
}
