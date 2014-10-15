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

// bring in Point and Size for easy reference

using awc::Point;
using awc::Size;


/*
 * Setup the comms and initialise the LCD
 */

void setup() {

  // initialise the I2C library for 400kHz
  
  Wire.begin();
  TWBR=12;
    
  // reset the coprocessor
  
  copro.reset();

  // setup the display and the timer pins to GPIO
  
  copro << awc::gamma(awc::Gamma::getDefault())                   // set default gamma
        << awc::background(awc::WHITE)                            // background is white
        << awc::clear()                                           // clear screen
        << awc::t1Gpio(awc::GPIOSLEW_50,awc::GPIODRIVE_PUSHPULL)  // T1 to GPIO mode
        << awc::t2Gpio(awc::GPIOSLEW_50,awc::GPIODRIVE_PUSHPULL)  // T2 to GPIO mode
        << awc::t1GpioReset()                                     // T1 to LOW
        << awc::t2GpioReset()                                     // T2 to LOW
        << awc::backlight(100);                                   // bring backlight to 100%

  // the backlight has a smooth rising curve, wait for it
  
  delay(500);
}


/*
 * Show a looping demo. We alternately set T1 and T2 at a
 * rate of 1Hz
 */

void loop() {
  
  static bool tstate=false;

  // go into 1 1Hz loop inverting the GPIO state

  setPins(tstate);
  tstate^=true;
  delay(1000);
}


/*
 * Set and show the current GPIO state
 */

void setPins(bool tstate) {

  // set pins

  copro << awc::t1GpioControl(tstate)
        << awc::t2GpioControl(!tstate);

  // update display
  
  showState(Copper::WIDTH/4,tstate ? awc::RED : awc::WHITE);
  showState((Copper::WIDTH/4)*3,tstate ? awc::WHITE : awc::RED);
}


/*
 * Show the current GPIO state as a red blob when HIGH. T1 is shown on
 * the left and T2 is on the right.
 */
  
void showState(int16_t center_x,uint32_t cr) {

  Point center(center_x,Copper::HEIGHT/2);
  Size size(Copper::WIDTH/4,Copper::HEIGHT/2);

  copro << awc::foreground(cr)                // set the foreground colour
        << awc::fillEllipse(center,size);     // fill the ellipse
}

