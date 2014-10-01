/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include "awcopper.h"
#include "WireStream.h"


namespace awc {

  /*
   * Static class member initialisation
   */

  uint8_t CoProcessor::buffer[BUFFER_LENGTH];


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
    if(count>0) {
      ::Wire.beginTransmission(SLAVE_ADDRESS);
      ::Wire.write(CoProcessor::buffer,count);
      ::Wire.endTransmission();
    }
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
   * Send the panel to sleep
   */

  uint16_t sleep() {
    *CoProcessor::buffer=cmd::SLEEP;
    return 1;
  }


  /*
   * Wake the panel up
   */

  uint16_t wake() {
    *CoProcessor::buffer=cmd::WAKE;
    return 1;
  }


  /*
   * Set the gamma for the panel
   */

  uint16_t gamma(const Gamma& gamma) {

    uint8_t *ptr=CoProcessor::buffer;

    *ptr++=cmd::GAMMA;
    
    memcpy(ptr,gamma._gamma,sizeof(gamma._gamma));
    return 1+sizeof(gamma._gamma);
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

    uint16_t rectangleOp(uint8_t operation,const Rectangle& rc) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=operation;

      *ptr++=rc.X;
      *ptr++=rc.X >> 8;

      *ptr++=rc.Y;
      *ptr++=rc.Y >> 8;

      *ptr++=rc.Width;
      *ptr++=rc.Width >> 8;

      *ptr++=rc.Height;
      *ptr++=rc.Height >> 8;

      return 9;
    }
  }

  uint16_t clearRectangle(const Rectangle& rc) {
    return rectangleOp(awc::cmd::CLEAR_RECTANGLE,rc);
  }

  uint16_t fillRectangle(const Rectangle& rc) {
    return rectangleOp(awc::cmd::FILL_RECTANGLE,rc);
  }

  uint16_t rectangle(const Rectangle& rc) {
    return rectangleOp(awc::cmd::RECTANGLE,rc);
  }


  /*
   * Line from (x1,y1) to (x2,y2)
   */

  uint16_t line(const Point& p1,const Point& p2) {

    uint8_t *ptr=CoProcessor::buffer;

    *ptr++=cmd::LINE;

    *ptr++=p1.X;
    *ptr++=p1.X >> 8;

    *ptr++=p1.Y;
    *ptr++=p1.Y >> 8;

    *ptr++=p2.X;
    *ptr++=p2.X >> 8;

    *ptr++=p2.Y;
    *ptr++=p2.Y >> 8;

    return 9;
  }
  

  /*
   * Draw a "polyline" which is a sequence of line segments connecting the
   * points that you supply in this method
   */

  uint16_t polyline(const Point *p,uint8_t count) {

    // check for nothing

    if(count==0)
      return 0;

    // command and count first

    WireStream stream(cmd::POLYLINE);

    stream.write(count);
    
    while(count--) {

      stream.write(p->X);
      stream.write(p->X >> 8);

      stream.write(p->Y);
      stream.write(p->Y >> 8);

      p++;
    }

    return 0;
  }


  /*
   * Select a new font
   */

  uint16_t font(FontId fid) {
    
    uint8_t *ptr=CoProcessor::buffer;

    *ptr++=cmd::FONT;
    *ptr=fid;

    return 2;
  }


  /*
   * Write a text string
   */

  uint16_t text(const Point& p,const char *str,TextMode textMode) {

    // set up the output stream

    WireStream stream(textMode==TRANSPARENT ? cmd::WRITE_TEXT : cmd::WRITE_FILLED_TEXT);

    // write the point

    stream.write(p.X);
    stream.write(p.X >> 8);

    stream.write(p.Y);
    stream.write(p.Y >> 8);

    // write out the string

    do {
      stream.write(*str);
    } while(*str++);

    // nothing for the operator to do, the stream takes care of it

    return 0;
  }
}
