#include <Wire.h>
#include <awcopper.h>

awc::CoProcessor copro;
using awc::Rectangle;
using awc::Point;

void setup() {

  Serial.begin(9600);
  
  // initialise the library
  
  copro.begin(awc::KHZ_400);

  // clear to black and enable the backlight
  
  copro << awc::background(awc::BLACK)
        << awc::clearRectangle(Rectangle(0,0,640,360))
        << awc::backlight(100); 
}

void loop() {
  lineDemo();
}


/*
 * Show a 5 second line demo
 */
 
void lineDemo() {

  uint16_t x1,y1,x2,y2;
  uint32_t start;
  
  prompt("Line demo");

  for(start=millis();millis()-start<5000;) {

    x1=random(639);
    y1=random(359);
    x2=random(639);
    y2=random(359);

    copro << awc::foreground(random(0xffffff))  // random colour foreground
          << awc::line(Point(x1,y1),Point(x2,y2));            // random line
  }
}


/*
 * Clear to black and show a prompt for 3 seconds
 */
 
void prompt(const char *str) {

  clearScreen();
  
  copro << awc::foreground(awc::WHITE)      // set foreground to white
        << awc::font(awc::ATARI)            // select the Atari font
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
        << awc::clearRectangle(Rectangle(0,0,640,360));
}

