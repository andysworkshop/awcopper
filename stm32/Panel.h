/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


/*
 * The panel class manages access to the LCD.
 */

class Panel {

  public:

    typedef Gpio16BitAccessMode<Panel,COLOURS_16BIT,48,42,42> LcdAccessMode;
    typedef R61523PwmBacklight<LcdAccessMode> LcdBacklight;
    typedef R61523_Landscape_64K_TypeB<LcdAccessMode> LcdPanel;

    enum {
      Port_DATA    = GPIOB_BASE,
      Port_CONTROL = GPIOA_BASE,

      Pin_RESET    = GPIO_Pin_0,
      Pin_WR       = GPIO_Pin_1,
      Pin_RS       = GPIO_Pin_2
    };

    enum {
      HEIGHT = LcdPanel::SHORT_SIDE,
      WIDTH = LcdPanel::LONG_SIDE
    };

    // declare the access mode carefully so that we pick up the optimised implementation

    typedef LcdPanel::tCOLOUR tCOLOUR;

  protected:
    LcdAccessMode *_accessMode;
    LcdPanel *_gl;
    LcdBacklight *_backlight;
    FontManager _fontManager;

  public:
    Panel();

    LcdPanel& getGraphicsLibrary();
    LcdAccessMode& getAccessMode();
    LcdBacklight& getBacklight();
    const FontManager& getFontManager() const;

    void clearScreen() const;
    void setBacklight(uint8_t newPercent);
};


/*
 * Get the font manager
 */

inline const FontManager& Panel::getFontManager() const {
  return _fontManager;
}


/*
 * Get the panel (graphics library) object
 */

inline Panel::LcdPanel& Panel::getGraphicsLibrary() {
  return *_gl;
}


/*
 * Get the access mode
 */

inline Panel::LcdAccessMode& Panel::getAccessMode() {
  return *_accessMode;
}


/*
 * Get the backlight object
 */

inline Panel::LcdBacklight& Panel::getBacklight() {
  return *_backlight;
}


/*
 * Clear to black
 */

inline void Panel::clearScreen() const {
  _gl->setBackground(ColourNames::BLACK);
  _gl->clearScreen();
}


/*
 * Set the new backlight percentage
 */

inline void Panel::setBacklight(uint8_t newPercent) {
  _backlight->setPercentage(newPercent);
}
