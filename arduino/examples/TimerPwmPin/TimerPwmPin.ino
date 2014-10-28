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

// bring in Point easy reference

using awc::Point;


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
  
  copro << awc::gamma(awc::Gamma::getDefault())   // set default gamma
        << awc::background(awc::WHITE)            // background is white
        << awc::clear()                           // clear screen
        << awc::t1Frequency(                      // use T1 in timer mode          
                  1999,                           // auto-reload at 2000
                  1,                              // prescaler of 2 (24/2 = 12MHz)
                  awc::CKDIV_1,                   // no clock-division
                  awc::COUNTERMODE_UP)            // timer counts up
        << awc::t1InitCompare(                    // initialise output channel
                  0,                              // start at 0 duty cycle
                  awc::OCMODE_PWM1,               // PWM mode 1
                  awc::OCPOLARITY_HIGH,           // output compare polarity is high
                  awc::OCPRELOAD_DISABLE)         // preloading is off
        << awc::t1Enable()                        // switch the timer pin on
        << awc::backlight(100);                   // bring backlight to 100%

  // the backlight has a smooth rising curve, wait for it
  
  delay(500);
}


/*
 * Vary the PWM duty cycle from 0..100% and back again. The timer has 2000 ticks
 * before reload. We vary the duty cycle by 10 every 20ms so it takes 4 seconds to
 * complete before reversing.
 */

void loop() {

  static int16_t compare=0,direction=1;

  // remove preview
  
  drawWave(compare,awc::WHITE);
  
  // check for reversal

  if((compare==0 && direction<0) || compare==2000 && direction>0)
    direction=-direction;
  
  // update duty cycle
  
  compare+=10*direction;
  copro << awc::t1SetCompare(compare);
  
  // redraw the wave
  
  drawWave(compare,awc::BLACK);
  
  // wait
  
  delay(20);
}


/*
 * Draw a preview of how the wave will look
 */
 
void drawWave(uint32_t compare,uint32_t cr) {

  Point p[4];

  // set up the points on the line
  
  p[0].X=0;
  p[0].Y=p[1].Y=(Copper::HEIGHT*3)/4;
  p[1].X=p[2].X=(Copper::WIDTH*compare)/2000;
  p[2].Y=p[3].Y=(Copper::HEIGHT/4);
  p[3].X=Copper::WIDTH-1;
  
  // draw the line
  
  copro << awc::foreground(cr)
        << awc::polyline(p,4);
}

