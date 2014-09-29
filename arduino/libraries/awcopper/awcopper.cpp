/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include "awcopper.h"


namespace awc {

  /*
   * Static class member initialisation
   */

  uint8_t CoProcessor::buffer[32];


  /*
   * Initialise the wire library
   */

  void CoProcessor::begin() {
    ::Wire.begin();
    reset();
  }


  /*
   * Write out the number of bytes already prepared in the static buffer
   */

  CoProcessor& CoProcessor::operator<<(uint16_t count) {

    ::Wire.beginTransmission(SLAVE_ADDRESS);
    ::Wire.write(CoProcessor::buffer,count);
    ::Wire.endTransmission();

    return *this;
  }


  /*
   * reset the STM32 with a zero length frame
   */

  void CoProcessor::reset() {
    ::Wire.beginTransmission(SLAVE_ADDRESS);
    ::Wire.endTransmission();

    delay(1000);
  }


  /*
   * Set the backlight to the given percentage level
   */

  uint16_t backlight(uint8_t percentage) {
    
    uint8_t *ptr=CoProcessor::buffer;

    *ptr++=cmd::SET_BACKLIGHT;
    *ptr=percentage;

    return 2;
  }


  /*
   * Colour operations
   */

  namespace {
  
    uint16_t colourOperation(uint8_t operation,uint32_t colorref) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=operation;

      *ptr++=colorref >> 16;    // RR
      *ptr++=colorref >> 8;     // GG
      *ptr++=colorref;          // BB

      return 4;
    }
  }

  uint16_t background(uint32_t colorref) {
    return colourOperation(awc::cmd::BACKGROUND_COLOUR,colorref);
  }

  uint16_t foreground(uint32_t colorref) {
    return colourOperation(awc::cmd::FOREGROUND_COLOUR,colorref);
  }


  /*
   * Rectangle operations
   */

  namespace {

    uint16_t rectangleOp(uint8_t operation,uint16_t x,uint16_t y,uint16_t width,uint16_t height) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=operation;

      *ptr++=x;
      *ptr++=x >> 8;

      *ptr++=y;
      *ptr++=y >> 8;

      *ptr++=width;
      *ptr++=width >> 8;

      *ptr++=height;
      *ptr++=height >> 8;

      return 9;
    }
  }

  uint16_t clearRectangle(uint16_t x,uint16_t y,uint16_t width,uint16_t height) {
    return rectangleOp(awc::cmd::CLEAR_RECTANGLE,x,y,width,height);
  }

  uint16_t fillRectangle(uint16_t x,uint16_t y,uint16_t width,uint16_t height) {
    return rectangleOp(awc::cmd::FILL_RECTANGLE,x,y,width,height);
  }

  uint16_t rectangle(uint16_t x,uint16_t y,uint16_t width,uint16_t height) {
    return rectangleOp(awc::cmd::RECTANGLE,x,y,width,height);
  }


  /*
   * Line from (x1,y1) to (x2,y2)
   */

  uint16_t line(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2) {

    uint8_t *ptr=CoProcessor::buffer;

    *ptr++=cmd::LINE;

    *ptr++=x1;
    *ptr++=x1 >> 8;

    *ptr++=y1;
    *ptr++=y1 >> 8;

    *ptr++=x2;
    *ptr++=x2 >> 8;

    *ptr++=y2;
    *ptr++=y2 >> 8;

    return 9;
  }
}
