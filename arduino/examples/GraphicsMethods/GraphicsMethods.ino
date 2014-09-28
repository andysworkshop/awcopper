#include <Wire.h>
#include <awcopper.h>

awc::CoProcessor copro;

void setup() {

  // initialise the library
  
  copro.begin();

  // clear to black and enable the backlight
  
  copro << awc::background(awc::BLACK)
        << awc::clearRectangle(0,0,640,360)
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
          << awc::line(x1,y1,x2,y2);            // random line
  }
}


/*
 * Clear to black and show a prompt for 3 seconds
 */
 
void prompt(const char *str) {
  copro << awc::clearRectangle(0,0,640,360)      // clear screen
        << awc::foreground(awc::WHITE);          // set foreground to white
}

