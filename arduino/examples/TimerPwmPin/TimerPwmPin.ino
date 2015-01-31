/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 *
 * This demo will generate 2 PWM waves of continuously varying duty
 * cycles using the T1 and T2 pins. The frequency of the signals will
 * be 16KHz on an overclocked MCU running at 64MHz.
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

  // setup the display and prompt
  
  copro << awc::gamma(awc::Gamma::getDefault())   // set default gamma
        << awc::background(awc::BLACK)            // background is white
        << awc::foreground(awc::WHITE)            // foreground is black
        << awc::font(awc::DOS)                    // set a font
        << awc::clear()                           // clear screen
        << awc::text(Point(10,10),                // show a prompt
                     "PWM Timer duty cycle demo");
                     
  // setup T1

  copro << awc::t1Frequency(                      // use T1 in timer mode          
                  1999,                           // auto-reload at 2000
                  1,                              // prescaler of 2 (24/32MHz (normal/overclocked))
                  awc::CKDIV_1,                   // no clock-division
                  awc::COUNTERMODE_UP)            // timer counts up
        << awc::t1InitCompare(                    // initialise output channel
                  0,                              // start at 0 duty cycle
                  awc::OCMODE_PWM1,               // PWM mode 1
                  awc::OCPOLARITY_HIGH,           // output compare polarity is high
                  awc::OCPRELOAD_DISABLE)         // preloading is off
        << awc::t1Enable();                       // switch the timer pin on

  // setup T2 the same as T1

  copro << awc::t2Frequency(                      // use T2 in timer mode          
                  1999,                           // auto-reload at 2000
                  1,                              // prescaler of 2 (24/32MHz (normal/overclocked))
                  awc::CKDIV_1,                   // no clock-division
                  awc::COUNTERMODE_UP)            // timer counts up
        << awc::t2InitCompare(                    // initialise output channel
                  0,                              // start at 0 duty cycle
                  awc::OCMODE_PWM1,               // PWM mode 1
                  awc::OCPOLARITY_HIGH,           // output compare polarity is high
                  awc::OCPRELOAD_DISABLE)         // preloading is off
        << awc::t2Enable();                       // switch the timer pin on


  // backlight on
  
  copro << awc::backlight(100);                   // bring backlight to 100%

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
  
  drawWave(compare,awc::BLACK,awc::BLACK);
  
  // check for reversal

  if((compare==0 && direction<0) || compare==2000 && direction>0)
    direction=-direction;
  
  // update duty cycle. T1 goes one way, T2 goes the other way
  
  compare+=10*direction;
  copro << awc::t1SetCompare(compare);
  copro << awc::t2SetCompare(2000-compare);
  
  // redraw the wave
  
  drawWave(compare,awc::YELLOW,awc::CYAN);
  
  // wait
  
  delay(20);
}


/*
 * Draw a preview of how the wave will look
 */
 
void drawWave(uint32_t compare,uint32_t cr1,uint32_t cr2) {

  Point p1[4],p2[4];

  // set up the points on the T1 line
  
  p1[0].X=0;
  p1[0].Y=p1[1].Y=(Copper::HEIGHT*2)/5;
  p1[1].X=p1[2].X=((Copper::WIDTH-1)*compare)/2000;
  p1[2].Y=p1[3].Y=(Copper::HEIGHT/5);
  p1[3].X=Copper::WIDTH-1;
  
  // set up the points on the T2 line
  
  p2[0].X=0;
  p2[0].Y=p2[1].Y=(Copper::HEIGHT*4)/5;
  p2[1].X=p2[2].X=((Copper::WIDTH-1)*(2000-compare))/2000;
  p2[2].Y=p2[3].Y=(Copper::HEIGHT*3)/5;
  p2[3].X=Copper::WIDTH-1;

  // draw the lines
  
  copro << awc::foreground(cr1)
        << awc::polyline(p1,4)
        << awc::foreground(cr2)
        << awc::polyline(p2,4);
}

