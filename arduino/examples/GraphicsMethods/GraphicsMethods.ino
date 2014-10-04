#include <Wire.h>
#include <awcopper.h>

// declare an instance of the coprocessor

typedef awc::LandscapeCoProcessor Copper;
Copper copro;

// bring in Rectangle, Point and Size for easy reference

using awc::Rectangle;
using awc::Point;
using awc::Size;


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
        << awc::backlight(100);                     // bring backlight to 100%

  // the backlight has a smooth rising curve, wait for it
  
  delay(500);
}

void loop() {
  clearDemo();
//  plotDemo();
 // polylineDemo();
//  basicColoursDemo();
//  lineDemo();
//  gradientDemo();
//  rectDemo();
//  ellipseDemo();
}


/*
 * Show a triangle
 */
 
void polylineDemo() {
}


/*
 * Show random coloured points for 10 seconds
 */

void plotDemo() {

  uint32_t start;
  Point p;
  
  prompt("plot points demo");
  
  for(start=millis();millis()-start<10000;) {
    
    p.X=random(Copper::WIDTH);
    p.Y=random(Copper::HEIGHT);
    
    copro << awc::foreground(random(awc::WHITE)) 
          << awc::plot(p);
  }
}


/*
 * Show a 5 second line demo
 */
 
void lineDemo() {

  uint16_t x1,y1,x2,y2;
  uint32_t start;
  
  prompt("Line demo");

  for(start=millis();millis()-start<5000;) {

    x1=random(Copper::WIDTH-1);
    y1=random(Copper::HEIGHT-1);
    x2=random(Copper::WIDTH-1);
    y2=random(Copper::HEIGHT-1);

    copro << awc::foreground(random(awc::WHITE))    // random colour foreground
          << awc::line(Point(x1,y1),Point(x2,y2));  // random line
  }
}


/*
 * Rapidly clear the screen for 5 seconds
 */
 
void clearDemo() {

  uint32_t start;

  prompt("Clear screen demo");

  for(start=millis();millis()-start<5000;) {

    copro << awc::background(random(awc::WHITE))
          << awc::clear();
  }
}


/*
 * Show each of the fully saturated colours for 500ms
 */
 
void basicColoursDemo() {

  uint8_t i;

  static const uint32_t colours[7]={
    awc::RED,
    awc::GREEN,
    awc::BLUE,
    awc::CYAN,
    awc::MAGENTA,
    awc::YELLOW,
    awc::BLACK,
  };

  prompt("Basic colours demo");

  for(i=0;i<sizeof(colours)/sizeof(colours[0]);i++) {
    
    copro << awc::background(colours[i])
          << awc::clear();
    
    delay(500);
  }
}


/*
 * Show random filled, outlined rectangles for 5 seconds each. 
 */
 
void rectDemo() {

  uint32_t start;
  Rectangle rc;
  bool filled;
  
  prompt("Rectangle demo");

  filled=true;
  
  for(start=millis();millis()-start<10000;) {

    rc.X=(random() % Copper::WIDTH/2);
    rc.Y=(random() % Copper::HEIGHT/2);
    rc.Width=random() % (Copper::WIDTH-rc.X-1);
    rc.Height=random() % (Copper::HEIGHT-rc.Y-1);

    if(filled && millis()-start>5000) {
      filled=false;
      copro << awc::clear();
    }
    
    copro << awc::foreground(random(awc::WHITE));
 
    if(filled)
      copro << awc::fillRectangle(rc);
    else
      copro << awc::rectangle(rc);
  }
}


/*
 * Show random filled, outlined ellipses for 5 seconds each. 
 */

void ellipseDemo() {

  Point p;
  Size s;
  uint32_t start;
  bool filled;

  prompt("Ellipse demo");

  filled=true;
  
  for(start=millis();millis()-start<10000;) {

    p.X=Copper::WIDTH/4+(random() % (Copper::WIDTH/2));
    p.Y=Copper::HEIGHT/4+(random() % (Copper::HEIGHT/2));

    if(p.X<Copper::WIDTH/2)
      s.Width=random() % p.X;
    else
      s.Width=random() % (Copper::WIDTH-p.X);

    if(p.Y<Copper::HEIGHT/2)
      s.Height=random() % p.Y;
    else
      s.Height=random() % (Copper::HEIGHT-p.Y);

    if(filled && millis()-start>5000) {
      filled=false;
      copro << awc::clear();
    }
    
    copro << awc::foreground(random(awc::WHITE));

    if(filled)
      copro << fillEllipse(p,s);
    else
      copro << ellipse(p,s);
  }
}


/*
 * Show some gradient fills
 */
 
void doGradientFills(awc::Direction dir) {

  Rectangle rc;
  uint16_t i;

  static uint32_t colours[7]={
    awc::RED,
    awc::GREEN,
    awc::BLUE,
    awc::CYAN,
    awc::MAGENTA,
    awc::YELLOW,
    awc::WHITE,
  };

  rc.Width=Copper::WIDTH;
  rc.Height=Copper::HEIGHT/2;

  for(i=0;i<sizeof(colours)/sizeof(colours[0]);i++) {
    
    rc.X=0;
    rc.Y=0;

    copro << awc::gradientFillRectangle(rc,dir,awc::BLACK,colours[i]);
    rc.Y=rc.Height;
    copro << awc::gradientFillRectangle(rc,dir,colours[i],awc::BLACK);

    delay(1000);
  }
}

void gradientDemo() {
  prompt("Gradient demo");
  
  doGradientFills(awc::HORIZONTAL);
  doGradientFills(awc::VERTICAL);
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
        << awc::clear();
}

