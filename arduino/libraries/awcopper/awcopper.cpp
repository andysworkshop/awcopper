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

  uint8_t CoProcessor::buffer[BUFFER_LENGTH];
  uint32_t CoProcessor::bytesRemaining=0;


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
   * stream in a variable number of bytes
   */

  CoProcessor& CoProcessor::operator<<(const Bytes& bytes) {

    byteStream.write(bytes._ptr,bytes._count);

    bytesRemaining-=bytes._count;
    
    if(bytesRemaining==0)
      byteStream.flush();

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
  
    uint8_t *addColourToBuffer(uint8_t *ptr,uint32_t cr) {

      *ptr++=cr >> 16;    // RR
      *ptr++=cr >> 8;     // GG
      *ptr++=cr;          // BB

      return ptr;
    }


    uint16_t colourOp(uint8_t operation,uint32_t cr) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=operation;
      addColourToBuffer(ptr,cr);

      return 4;
    }
  }

  uint16_t background(uint32_t colorref) {
    return colourOp(awc::cmd::BACKGROUND_COLOUR,colorref);
  }

  uint16_t foreground(uint32_t colorref) {
    return colourOp(awc::cmd::FOREGROUND_COLOUR,colorref);
  }


  /*
   * Rectangle operations
   */

  namespace {

    uint8_t *addRectToBuffer(uint8_t *ptr,const Rectangle& rc) {

      *ptr++=rc.X;
      *ptr++=rc.X >> 8;

      *ptr++=rc.Y;
      *ptr++=rc.Y >> 8;

      *ptr++=rc.Width;
      *ptr++=rc.Width >> 8;

      *ptr++=rc.Height;
      *ptr++=rc.Height >> 8;

      return ptr;
    }


    uint16_t rectangleOp(uint8_t operation,const Rectangle& rc) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=operation;
      addRectToBuffer(ptr,rc);

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
   * Clear the whole screen to the current background colour
   */

  uint16_t clear() {
    *CoProcessor::buffer=cmd::CLEAR_SCREEN;
    return 1;
  }


  /*
   * Fill a rectangle with a gradient
   */

  uint16_t gradientFillRectangle(const Rectangle& rc,Direction dir,uint32_t firstColour,uint32_t lastColour) {

    uint8_t *ptr=CoProcessor::buffer;

    *ptr++=awc::cmd::GRADIENT_FILL_RECTANGLE;

    ptr=addRectToBuffer(ptr,rc);              // 8 bytes
    *ptr++=dir;
    ptr=addColourToBuffer(ptr,firstColour);   // 3 bytes
    addColourToBuffer(ptr,lastColour);        // 3 bytes

    return 16;
  }


  /*
   * Plot a single point in the foreground colour 
   */

  uint16_t plot(const Point& p) {

    uint8_t *ptr=CoProcessor::buffer;

    *ptr++=cmd::PLOT;

    *ptr++=p.X;
    *ptr++=p.X >> 8;

    *ptr++=p.Y;
    *ptr++=p.Y >> 8;

    return 5;
  }


  /*
   * Ellipse operations
   */

  namespace {

    uint16_t ellipseOp(uint8_t operation,const Point& center,const Size& size) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=operation;
      addRectToBuffer(ptr,Rectangle(center,size));

      return 9;
    }
  }


  /*
   * Draw an ellipse centered at [center] and with radii of [size]
   */

  uint16_t ellipse(const Point& center,const Size& size) {
    return ellipseOp(cmd::ELLIPSE,center,size);
  }
  

  /*
   * Fill an ellipse centered at [center] and with radii of [size]
   */

  uint16_t fillEllipse(const Point& center,const Size& size) {
    return ellipseOp(cmd::FILL_ELLIPSE,center,size);
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


  /*
   * Issue the command to begin writing raw data. This must be followed by one or more
   * calls to writeData() to actually transfer it
   */

  uint16_t beginWriting() {
    *CoProcessor::buffer=cmd::BEGIN_WRITING;
    return 1;
  }


  /*
   * Write a block of raw data bytes to the display
   */

  uint16_t writeData(const void *data,uint16_t count) {

    WireStream stream(cmd::WRITE_DATA);

    stream.write(data,count);
    return 0;
  }


  /*
   * Erase the flash device
   */

  uint16_t eraseFlash() {
    *CoProcessor::buffer=cmd::ERASE_FLASH_DEVICE;
    return 1;
  }


  /*
   * Flash sector erase operations. Addresses are 24-bit
   */

  namespace {

    uint16_t eraseOp(uint8_t operation,uint32_t address) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=cmd::ERASE_FLASH_SECTOR;
      *ptr++=operation;
      *ptr++=address;
      *ptr++=address >> 8;
      *ptr++=address >> 16;

      return 5;
    }
  }


  /*
   * Erase a 4Kb sector
   */

  uint16_t erase4KSector(uint32_t address) {
    return eraseOp(4,address);
  }


  /*
   * Erase an 8Kb sector
   */

  uint16_t erase8KSector(uint32_t address) {
    return eraseOp(8,address);
  }


  /*
   * Erase a 64Kb sector
   */

  uint16_t erase64KSector(uint32_t address) {
    return eraseOp(64,address);
  }


  /*
   * Program a page in the flash device. A page is 256 bytes. This command must
   * be followed by 256 bytes of data streamed into the coprocessor
   */

  uint16_t program(uint32_t address) {

    uint8_t *ptr=CoProcessor::buffer;

    // buffer the command and the address

    *ptr++=cmd::PROGRAM_FLASH;
    *ptr++=address;
    *ptr++=address >> 8;
    *ptr++=address >> 16;

    // await 256 bytes of page data

    CoProcessor::bytesRemaining=256;

    // 4 bytes written here

    return 4;
  }


  /*
   * Begin transmitting a JPEG from the Arduino to the STM32
   */

  uint16_t jpeg(const Rectangle& rc,uint32_t count) {

    uint8_t *ptr=CoProcessor::buffer;

    // buffer the command and the rectangle

    *ptr++=cmd::WRITE_JPEG;
    ptr=addRectToBuffer(ptr,rc);

    // 24 bit size

    *ptr++=count;
    *ptr++=count >> 8;
    *ptr++=count >> 16;

    // await the data

    CoProcessor::bytesRemaining=count;

    // 12 bytes written by this method

    return 12;
  }
}
