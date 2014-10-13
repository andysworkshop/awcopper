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
  JPEG_SIZE=0,
  
  BITMAP_ADDRESS=0,
  BITMAP_SIZE=0,

  LZG_ADDRESS=0,
  LZG_SIZE=0  
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
  
  uint32_t start,elapsed;
  const Rectangle fullScreen(0,0,Copper::WIDTH,Copper::HEIGHT);
  
  prompt("JPEG demo from coprocessor external flash");
  
  start=millis();
  copro << awc::jpegFlash(fullScreen,JPEG_ADDRESS,JPEG_SIZE);
  elapsed=millis()-start;
  
  // enjoy for 8 seconds :)
  
  delay(8000);

  // show how long it took to display
  
  showCount(1,elapsed,"jpeg image");
}


/*
 * Display a full-screen uncompressed image
 */

void bitmapDemo() {

  uint32_t start,elapsed;
  const Rectangle fullScreen(0,0,Copper::WIDTH,Copper::HEIGHT);

  prompt("Uncompressed bitmap demo");

  start=millis();
  copro << awc::bitmapFlash(fullScreen,BITMAP_ADDRESS,BITMAP_SIZE);
  elapsed=millis()-start;
  
  // enjoy for 8 seconds :)
  
  delay(8000);

  // show how long it took to display
  
  showCount(1,elapsed,"bitmap image");
}


/*
 * Display a full-screen uncompressed image
 */

void lzgDemo() {

  uint32_t start,elapsed;
  const Rectangle fullScreen(0,0,Copper::WIDTH,Copper::HEIGHT);

  prompt("LZG Compressed bitmap demo");

  start=millis();
  copro << awc::lzgBitmapFlash(fullScreen,LZG_ADDRESS,LZG_SIZE);
  elapsed=millis()-start;
  
  // enjoy for 8 seconds :)
  
  delay(8000);

  // show how long it took to display
  
  showCount(1,elapsed,"LZG bitmap image");
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
 * Show a basic post-demo count display
 */
 
void showCount(uint32_t count,uint32_t ms,const char *what) {

  char buffer[100];
  
  sprintf(buffer,"%lu %s in %lu milliseconds",count,what,ms);
  
  clearScreen();
  
  copro << awc::foreground(awc::WHITE)       // set foreground to white
        << awc::font(awc::ATARI)             // select the Atari font
        << awc::text(Point::Origin,buffer);  // text string at the origin

  delay(3000);
}


/*
 * Clear the screen to black
 */
 
void clearScreen() {
  
  // clear screen
 
  copro << awc::background(awc::BLACK)
        << awc::clear();
}

