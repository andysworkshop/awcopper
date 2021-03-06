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

// references to the JPEG data

extern const uint32_t JpegData,JpegDataSize;


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
        << awc::backlight(100);                     // bring backlight to 100%

  // the backlight has a smooth rising curve, wait for it
  
  delay(500);
}


/*
 * Show a looping demo
 */

void loop() {
  jpegDemo();
  ellipseDemo();
  textDemo();
  fontDemo();
  polylineDemo();
  clearDemo();
  plotDemo();
  basicColoursDemo();
  lineDemo();
  gradientDemo();
  rectDemo();
  sleepDemo();
}

/*
 * Show a large JPEG that's compiled into this sketch
 */
 
void jpegDemo() {

  uint16_t size,batchSize,i;
  const uint8_t *data;
  uint8_t buffer[128],*bufptr;      // multiple of 32 makes the most of the Wire buffer
  uint32_t start,elapsed;
  
  prompt("JPEG demo uploaded from the Arduino");
  
  // the background should be white
  
  copro << awc::background(awc::WHITE)                    // white background
        << awc::clear()                                   // clear down  
        << awc::font(awc::KYROU_BOLD)                     // select a font
        << awc::foreground(awc::BLACK)                    // set a font colour
        << awc::text(Point(10,10),"Got one of these?")    // draw a title
        << awc::text(Point(10,340),"Get Andy's CoProcessor shield for cheap high performance graphics");
        
  // prepare for the upload

  data=reinterpret_cast<const uint8_t *>(&JpegData);
  size=reinterpret_cast<uint16_t>(&JpegDataSize);
  
  // draw the JPEG centered on the screen
  
  copro << awc::jpeg(Rectangle(70,26,500,308),size);

  // send the expected data in batches

  for(start=millis();size;size-=batchSize) {
    
    batchSize=size<sizeof(buffer) ? size : sizeof(buffer);
    bufptr=buffer;
    
    for(i=batchSize;i;i--)
      *bufptr++=pgm_read_byte_near(data++);
    
    copro << awc::Bytes(buffer,batchSize);
  }

  elapsed=millis()-start;
  
  // enjoy for 10 seconds :)
  
  delay(10000);
  
  // show how long it took to display
  
  showCount(1,elapsed,"jpeg image");
}


/*
 * Demonstrate putting the panel to sleep
 */
 
void sleepDemo() {

  prompt("Sleep demo");

  // show the sleeping message

  copro << awc::foreground(awc::WHITE)
        << awc::font(awc::DOS)
        << awc::text(Point::Origin,"Sleeping now...");
        
  delay(3000);
  
  // go to sleep for 3 seconds
  
  copro << awc::sleep();
  delay(3000);
  
  // wake up
  
  copro << awc::wake()
        << awc::clear()
        << awc::text(Point::Origin,"Woken up again...");
        
  delay(3000);
}


/*
 * Show random text repeatedly
 */
 
void textDemo() {
     
  Size size;
  uint32_t start,count;
  Point p;
  
  prompt("Text demo");

  copro << font(awc::PROGGY);
  count=0;
  
  for(start=millis();millis()-start<5000;) {
  
    p.X=random(Copper::WIDTH-200);
    p.Y=random(Copper::HEIGHT-16);
    
    copro << awc::foreground(random(awc::WHITE))
          << awc::text(p,"The quick brown fox jumped over the lazy dogs",awc::SOLID);

    count++;
  }
  
  showCount(count,5000,"text strings");
}


/*
 * Show all available fonts for a few seconds
 */
 
void fontDemo() {

  static struct {
    awc::FontId id;
    const char *name;
  } const fdefs[]= {
    { awc::APPLE,      "APPLE" },
    { awc::ATARI,      "ATARI" },
    { awc::DOS,        "DOS" },
    { awc::KYROU_BOLD, "KYROU_BOLD" },
    { awc::KYROU,      "KYROU" },
    { awc::NINTENDO,   "NINTENDO" },
    { awc::PIXELADE,   "PIXELADE" },
    { awc::PROGGY,     "PROGGY" },
    { awc::GOLDFISH,   "GOLDFISH" } 
  };
  
  prompt("font demo");
  char buffer[100];
  uint16_t y,i;
  
  copro << awc::foreground(awc::WHITE);

  y=50;
  for(i=0;i<sizeof(fdefs)/sizeof(fdefs[0]);i++) {
  
    strcpy(buffer,fdefs[i].name);
    strcat(buffer,": the quick brown fox jumped over the lazy dogs");
  
    copro << awc::font(fdefs[i].id)
          << awc::text(Point(100,y),buffer);
        
    y+=16;
  }
  delay(10000);
}


/*
 * Show a polyline demo
 */
 
void polylineDemo() {
  
  Point p[5];
  uint32_t cr,start,total;
  int16_t disp;
  uint16_t count;
  
  prompt("polyline demo");
  
  p[0].X=0;
  p[0].Y=p[2].Y=p[4].Y=0;
  p[1].X=Copper::WIDTH/4;
  p[1].Y=p[3].Y=Copper::HEIGHT-1;
  p[2].X=Copper::WIDTH/2;
  p[3].X=Copper::WIDTH/4*3;
  p[4].X=Copper::WIDTH-1;
  
  cr=awc::RED;
  count=0;
  disp=2;
  total=0;
  
  for(start=millis();millis()-start<5000;) {
    
    copro << awc::foreground(cr)
          << awc::polyline(p,5);
          
    p[1].X+=disp;
    p[3].X-=disp;
    
    if(count++==Copper::WIDTH/2) {
      count=0;
      disp=-disp;
    }
    
    cr+=4;
    total++;
  }
  
  showCount(total,5000,"polylines");
}


/*
 * Show random coloured points for 10 seconds
 */

void plotDemo() {

  uint32_t start,count;
  Point p;
  
  prompt("plot points demo");
  
  count=0;
  for(start=millis();millis()-start<10000;) {
    
    p.X=random(Copper::WIDTH);
    p.Y=random(Copper::HEIGHT);
    
    copro << awc::foreground(random(awc::WHITE)) 
          << awc::plot(p);
          
    count++;
  }
  
  showCount(count,10000,"coloured points");
}


/*
 * Show a 5 second line demo
 */
 
void lineDemo() {

  uint16_t x1,y1,x2,y2;
  uint32_t start,count;
  
  prompt("Line demo");

  count=0;
  for(start=millis();millis()-start<5000;) {

    x1=random(Copper::WIDTH-1);
    y1=random(Copper::HEIGHT-1);
    x2=random(Copper::WIDTH-1);
    y2=random(Copper::HEIGHT-1);

    copro << awc::foreground(random(awc::WHITE))    // random colour foreground
          << awc::line(Point(x1,y1),Point(x2,y2));  // random line

    count++;
  }
  showCount(count,5000,"lines");
}


/*
 * Rapidly clear the screen for 5 seconds
 */
 
void clearDemo() {

  uint32_t start,count;

  prompt("Clear screen demo");

  count=0;
  for(start=millis();millis()-start<5000;) {

    copro << awc::background(random(awc::WHITE))
          << awc::clear();

    count++;
  }
  
  delay(3000);    // catch up!
  showCount(count,5000,"screen clearances");
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

  uint32_t start,count;
  Rectangle rc;
  bool filled;
  
  prompt("Rectangle demo");

  filled=true;
  count=0;
  
  for(start=millis();millis()-start<10000;) {

    rc.X=random(Copper::WIDTH/2);
    rc.Y=random(Copper::HEIGHT/2);
    rc.Width=random(1,Copper::WIDTH-rc.X-1);
    rc.Height=random(1,Copper::HEIGHT-rc.Y-1);

    if(filled && millis()-start>5000) {
      filled=false;
      copro << awc::clear();
    }
    
    copro << awc::foreground(random(awc::WHITE));
 
    if(filled)
      copro << awc::fillRectangle(rc);
    else
      copro << awc::rectangle(rc);
  
    count++;
  }
  
  showCount(count,5000,"rectangles");
}


/*
 * Show random filled, outlined ellipses for 5 seconds each. 
 */

void ellipseDemo() {

  Point p;
  Size s;
  uint32_t start,count;
  bool filled;

  prompt("Ellipse demo");

  filled=true;
  count=0;
  
  for(start=millis();millis()-start<10000;) {

    p.X=(Copper::WIDTH/4)+random(Copper::WIDTH/2);
    p.Y=(Copper::HEIGHT/4)+random(Copper::HEIGHT/2);

    if(p.X<Copper::WIDTH/2)
      s.Width=random(1,p.X);
    else
      s.Width=random(1,Copper::WIDTH-p.X);

    if(p.Y<Copper::HEIGHT/2)
      s.Height=random(1,p.Y);
    else
      s.Height=random(1,Copper::HEIGHT-p.Y);

    if(filled && millis()-start>5000) {
      filled=false;
      copro << awc::clear();
    }
    
    copro << awc::foreground(random(awc::WHITE));

    if(filled)
      copro << fillEllipse(p,s);
    else
      copro << ellipse(p,s);
  
    count++;
  }
  showCount(count,5000,"ellipses");
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

