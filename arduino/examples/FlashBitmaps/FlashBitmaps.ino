/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */
 
#include <Wire.h>
#include <awcopper.h>

// declare an instance of the coprocessor

typedef awc::LandscapeCoProcessor Copper;
Copper copro;

// bring in Rectangle, Point and Size for easy reference

using awc::Rectangle;
using awc::Point;
using awc::Size;

// addresses and sizes in flash of the 3 graphics

enum {
  JPEG_ADDRESS=0,
  JPEG_SIZE=97548,
  
  BITMAP_ADDRESS=97792,
  BITMAP_SIZE=460800,

  LZG_ADDRESS=558592,
  LZG_SIZE=255682
};


/*
 * Setup the comms and initialise the LCD
 */

void setup() {

  // initialise the I2C library for 400kHz
  
  Wire.begin();
  TWBR=12;
    
  // reset the coprocessor
  
  copro.reset();

  // clear to black and enable the backlight
  
  copro << awc::gamma(awc::Gamma::getDefault())     // set default gamma
        << awc::background(awc::BLACK)              // background is black
        << awc::clear()                             // clear screen
        << awc::font(awc::ATARI)                    // select the font we use throughout
        << awc::backlight(100);                     // bring backlight to 100%

  // the backlight has a smooth rising curve, wait for it
  
  delay(500);
}


/*
 * Show a looping demo
 */

void loop() {
  jpegDemo();
  bitmapDemo();
  lzgDemo();
}


/*
 * Display a full-screen JPEG
 */

void jpegDemo() {
  
//  const Rectangle fullScreen(0,0,Copper::WIDTH,Copper::HEIGHT);
  const Rectangle fullScreen(0,0,500,308);
  
  prompt("JPEG demo from coprocessor external flash");
  
  copro << awc::jpegFlash(fullScreen,JPEG_SIZE,JPEG_ADDRESS);
  
  // enjoy for 8 seconds :)
  
  delay(8000);
}


/*
 * Display a full-screen uncompressed image
 */

void bitmapDemo() {

  const Rectangle fullScreen(0,0,Copper::WIDTH,Copper::HEIGHT);

  prompt("Uncompressed bitmap demo");

  copro << awc::bitmapFlash(fullScreen,BITMAP_SIZE,BITMAP_ADDRESS);
  
  // enjoy for 8 seconds :)
  
  delay(8000);
}


/*
 * Display a full-screen uncompressed image
 */

void lzgDemo() {

  uint32_t start,elapsed;
  const Rectangle fullScreen(0,0,Copper::WIDTH,Copper::HEIGHT);

  prompt("LZG Compressed bitmap demo");
  copro << awc::lzgBitmapFlash(fullScreen,LZG_SIZE,LZG_ADDRESS);
  
  // enjoy for 8 seconds :)
  
  delay(8000);
}

 
/*
 * Show a prompt for 3 seconds
 */

void prompt(const char *str) {

  clearScreen();
  
  copro << awc::foreground(awc::WHITE)      // set foreground to white
        << awc::text(Point::Origin,str);    // text string at the origin
 
  // wait for 3 secs
 
  delay(3000);
 
  // clear and return
 
  clearScreen();
}


/*
 * Clear the screen to black
 */
 
void clearScreen() {
  
  // clear screen
 
  copro << awc::background(awc::BLACK)
        << awc::clear();
}

