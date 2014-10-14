/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

namespace awc {

  /*
   * Forward declarations
   */

  uint16_t backlight(uint8_t percentage);
  uint16_t window(const Rectangle& rc);
  uint16_t sleep();
  uint16_t wake();
  uint16_t gamma(const Gamma& gamma);
  uint16_t beginWriting();
  uint16_t writeData(const void *data,uint16_t count);

  uint16_t background(uint32_t colorref);
  uint16_t foreground(uint32_t colorref);

  uint16_t clear();
  uint16_t clearRectangle(const Rectangle& rc);
  uint16_t fillRectangle(const Rectangle& rc);
  uint16_t rectangle(const Rectangle& rc);
  uint16_t gradientFillRectangle(const Rectangle& rc,Direction dir,uint32_t firstColour,uint32_t lastColour);

  uint16_t line(const Point& p1,const Point& p2);
  uint16_t polyline(const Point *p,uint8_t count);

  uint16_t font(FontId fid);
  uint16_t text(const Point& p,const char *str,TextMode textMode=TRANSPARENT);

  uint16_t plot(const Point& p);

  uint16_t ellipse(const Point& center,const Size& size);
  uint16_t fillEllipse(const Point& center,const Size& size);

  uint16_t eraseFlash();
  uint16_t erase4KSector(uint32_t address);
  uint16_t erase8KSector(uint32_t address);
  uint16_t erase64KSector(uint32_t address);
  uint16_t program(uint32_t address);

  uint16_t jpeg(const Rectangle& rc,uint32_t count);
  uint16_t bitmap(const Rectangle& rc,uint32_t count);
  uint16_t lzgBitmap(const Rectangle& rc,uint32_t count);

  uint16_t jpegFlash(const Rectangle& rc,uint32_t count,uint32_t address);
  uint16_t bitmapFlash(const Rectangle& rc,uint32_t count,uint32_t address);
  uint16_t lzgBitmapFlash(const Rectangle& rc,uint32_t count,uint32_t address);

  uint16_t t1Frequency(uint32_t period,uint16_t prescaler,ClockDivision clockDivision,CounterMode counterMode);
  uint16_t t2Frequency(uint32_t period,uint16_t prescaler,ClockDivision clockDivision,CounterMode counterMode);

  uint16_t t1InitCompare(uint32_t compareValue,OcMode ocMode,OcPolarity polarity,OcPreload preload);
  uint16_t t2InitCompare(uint32_t compareValue,OcMode ocMode,OcPolarity polarity,OcPreload preload);

  uint16_t t1SetCompare(uint32_t compareValue);
  uint16_t t2SetCompare(uint32_t compareValue);

  uint16_t t1Enable();
  uint16_t t1Disable();
  uint16_t t2Enable();
  uint16_t t2Disable();

  uint16_t t1Gpio(GpioSlew slew,GpioDrive drive);
  uint16_t t2Gpio(GpioSlew slew,GpioDrive drive);

  uint16_t t1GpioSet();
  uint16_t t2GpioSet();
  uint16_t t1GpioReset();
  uint16_t t2GpioReset();
  uint16_t t1GpioControl(bool set);
  uint16_t t2GpioControl(bool set);


  /*
   * Write out the number of bytes already prepared in the static buffer
   */

  inline CoProcessor& CoProcessor::operator<<(uint16_t count) {
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

  inline CoProcessor& CoProcessor::operator<<(const Bytes& bytes) {

    byteStream.write(bytes._ptr,bytes._count);

    bytesRemaining-=bytes._count;
    
    if(bytesRemaining==0)
      byteStream.flush();

    return *this;
  }
 

  /*
   * reset the STM32 with a zero length frame
   */

  inline void CoProcessor::reset() {
    ::Wire.beginTransmission(SLAVE_ADDRESS);
    ::Wire.endTransmission();

    delay(1000);
  }


  /*
   * Set the backlight to the given percentage level
   */

  inline uint16_t backlight(uint8_t percentage) {
    
    uint8_t *ptr=CoProcessor::buffer;

    *ptr++=cmd::SET_BACKLIGHT;
    *ptr=percentage;

    return 2;
  }
  

  /*
   * Send the panel to sleep
   */

  inline uint16_t sleep() {
    *CoProcessor::buffer=cmd::SLEEP;
    return 1;
  }


  /*
   * Wake the panel up
   */

  inline uint16_t wake() {
    *CoProcessor::buffer=cmd::WAKE;
    return 1;
  }


  /*
   * Set the gamma for the panel
   */

  inline uint16_t gamma(const Gamma& gamma) {

    uint8_t *ptr=CoProcessor::buffer;

    *ptr++=cmd::GAMMA;
    
    memcpy(ptr,gamma._gamma,sizeof(gamma._gamma));
    return 1+sizeof(gamma._gamma);
  }


  /*
   * Colour operations
   */

  namespace internal {


    /*
     * add a 24-bit colour value to a buffer
     */

    inline uint8_t *addColourToBuffer(uint8_t *ptr,uint32_t cr) {

      *ptr++=cr >> 16;    // RR
      *ptr++=cr >> 8;     // GG
      *ptr++=cr;          // BB

      return ptr;
    }


    /*
     * Generic colour (bg/fg) operation
     */

    inline uint16_t colourOp(uint8_t operation,uint32_t cr) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=operation;
      addColourToBuffer(ptr,cr);

      return 4;
    }
  }

  inline uint16_t background(uint32_t colorref) {
    return internal::colourOp(awc::cmd::BACKGROUND_COLOUR,colorref);
  }

  inline uint16_t foreground(uint32_t colorref) {
    return internal::colourOp(awc::cmd::FOREGROUND_COLOUR,colorref);
  }


  /*
   * Rectangle operations
   */

  namespace internal {

    /*
     * Add an 8 byte rectangle to a buffer (x,y,w,h)
     */

    inline uint8_t *addRectToBuffer(uint8_t *ptr,const Rectangle& rc) {

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

    /*
     * Generic rectangle operation (clear/fill/outline)
     */

    inline uint16_t rectangleOp(uint8_t operation,const Rectangle& rc) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=operation;
      addRectToBuffer(ptr,rc);

      return 9;
    }
  }


  /*
   * Clear a rectangle to the background colour
   */

  inline uint16_t clearRectangle(const Rectangle& rc) {
    return internal::rectangleOp(awc::cmd::CLEAR_RECTANGLE,rc);
  }


  /*
   * Fill a rectangle with the foreground colour
   */

  inline uint16_t fillRectangle(const Rectangle& rc) {
    return internal::rectangleOp(awc::cmd::FILL_RECTANGLE,rc);
  }


  /*
   * Draw an outline of a rectangle
   */

  inline uint16_t rectangle(const Rectangle& rc) {
    return internal::rectangleOp(awc::cmd::RECTANGLE,rc);
  }


  /*
   * Clear the whole screen to the current background colour
   */

  inline uint16_t clear() {
    *CoProcessor::buffer=cmd::CLEAR_SCREEN;
    return 1;
  }


  /*
   * Fill a rectangle with a gradient
   */

  inline uint16_t gradientFillRectangle(const Rectangle& rc,Direction dir,uint32_t firstColour,uint32_t lastColour) {

    uint8_t *ptr=CoProcessor::buffer;

    *ptr++=awc::cmd::GRADIENT_FILL_RECTANGLE;

    ptr=internal::addRectToBuffer(ptr,rc);              // 8 bytes
    *ptr++=dir;
    ptr=internal::addColourToBuffer(ptr,firstColour);   // 3 bytes
    internal::addColourToBuffer(ptr,lastColour);        // 3 bytes

    return 16;
  }


  /*
   * Plot a single point in the foreground colour 
   */

  inline uint16_t plot(const Point& p) {

    uint8_t *ptr=CoProcessor::buffer;

    *ptr++=cmd::PLOT;

    *ptr++=p.X;
    *ptr++=p.X >> 8;

    *ptr++=p.Y;
    *ptr=p.Y >> 8;

    return 5;
  }


  /*
   * Ellipse operations
   */

  namespace internal {

    inline uint16_t ellipseOp(uint8_t operation,const Point& center,const Size& size) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=operation;
      addRectToBuffer(ptr,Rectangle(center,size));

      return 9;
    }
  }


  /*
   * Draw an ellipse centered at [center] and with radii of [size]
   */

  inline uint16_t ellipse(const Point& center,const Size& size) {
    return internal::ellipseOp(cmd::ELLIPSE,center,size);
  }
  

  /*
   * Fill an ellipse centered at [center] and with radii of [size]
   */

  inline uint16_t fillEllipse(const Point& center,const Size& size) {
    return internal::ellipseOp(cmd::FILL_ELLIPSE,center,size);
  }


  /*
   * Line from (x1,y1) to (x2,y2)
   */

  inline uint16_t line(const Point& p1,const Point& p2) {

    uint8_t *ptr=CoProcessor::buffer;

    *ptr++=cmd::LINE;

    *ptr++=p1.X;
    *ptr++=p1.X >> 8;

    *ptr++=p1.Y;
    *ptr++=p1.Y >> 8;

    *ptr++=p2.X;
    *ptr++=p2.X >> 8;

    *ptr++=p2.Y;
    *ptr=p2.Y >> 8;

    return 9;
  }
  

  /*
   * Draw a "polyline" which is a sequence of line segments connecting the
   * points that you supply in this method
   */

  inline uint16_t polyline(const Point *p,uint8_t count) {

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

  inline uint16_t font(FontId fid) {
    
    uint8_t *ptr=CoProcessor::buffer;

    *ptr++=cmd::FONT;
    *ptr=fid;

    return 2;
  }


  /*
   * Write a text string
   */

  inline uint16_t text(const Point& p,const char *str,TextMode textMode) {

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

  inline uint16_t beginWriting() {
    *CoProcessor::buffer=cmd::BEGIN_WRITING;
    return 1;
  }


  /*
   * Write a block of raw data bytes to the display
   */

  inline uint16_t writeData(const void *data,uint16_t count) {

    WireStream stream(cmd::WRITE_DATA);

    stream.write(data,count);
    return 0;
  }


  /*
   * Erase the flash device
   */

  inline uint16_t eraseFlash() {
    *CoProcessor::buffer=cmd::ERASE_FLASH_DEVICE;
    return 1;
  }


  /*
   * Flash sector erase operations. Addresses are 24-bit
   */

  namespace internal {

    inline uint16_t eraseOp(uint8_t operation,uint32_t address) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=cmd::ERASE_FLASH_SECTOR;
      *ptr++=operation;
      *ptr++=address;
      *ptr++=address >> 8;
      *ptr=address >> 16;

      return 5;
    }
  }


  /*
   * Erase a 4Kb sector
   */

  inline uint16_t erase4KSector(uint32_t address) {
    return internal::eraseOp(4,address);
  }


  /*
   * Erase an 8Kb sector
   */

  inline uint16_t erase8KSector(uint32_t address) {
    return internal::eraseOp(8,address);
  }


  /*
   * Erase a 64Kb sector
   */

  inline uint16_t erase64KSector(uint32_t address) {
    return internal::eraseOp(64,address);
  }


  /*
   * Program a page in the flash device. A page is 256 bytes. This command must
   * be followed by 256 bytes of data streamed into the coprocessor
   */

  inline uint16_t program(uint32_t address) {

    uint8_t *ptr=CoProcessor::buffer;

    // buffer the command and the address

    *ptr++=cmd::PROGRAM_FLASH;
    *ptr++=address;
    *ptr++=address >> 8;
    *ptr=address >> 16;

    // await 256 bytes of page data

    CoProcessor::bytesRemaining=256;

    // 4 bytes written here

    return 4;
  }


  /*
   * General bitmap operations
   */

  namespace internal {

    inline uint16_t bitmapOp(uint8_t operation,const Rectangle& rc,uint32_t count) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=operation;
      ptr=addRectToBuffer(ptr,rc);

      // 24 bit size

      *ptr++=count;
      *ptr++=count >> 8;
      *ptr=count >> 16;

      // await the data

      CoProcessor::bytesRemaining=count;

      // 12 bytes written by this method

      return 12;
    }
  }


  /*
   * Begin transmitting a JPEG from the Arduino to the STM32
   */

  inline uint16_t jpeg(const Rectangle& rc,uint32_t count) {
    return internal::bitmapOp(cmd::WRITE_JPEG,rc,count);
  }


  /*
   * Begin transmitting an uncompressed bitmap from the Arduino to the STM32. The limited memory
   * of the Arduino makes this rather an unappealing option.
   */

  inline uint16_t bitmap(const Rectangle& rc,uint32_t count) {
    return internal::bitmapOp(cmd::WRITE_BITMAP,rc,count);
  }


  /*
   * Begin transmitting an LZG compressed bitmap from the Arduino to the STM32. LZG
   * compression does very well on the same type of grapics where PNG does well.
   */

  inline uint16_t lzgBitmap(const Rectangle& rc,uint32_t count) {
    return internal::bitmapOp(cmd::WRITE_LZG_BITMAP,rc,count);
  }


  /*
   * Flash bitmap operations
   */

  namespace internal {

    inline uint16_t flashBitmapOp(uint8_t operation,const Rectangle& rc,uint32_t count,uint32_t address) {

      uint8_t *ptr;

      bitmapOp(operation,rc,count);
      ptr=CoProcessor::buffer+12;

      *ptr++=address;
      *ptr++=address >> 8;
      *ptr=address >> 16;

      return 15;
    }
  }


  /*
   * Write a JPEG stored in the CoProcessor external flash IC to the display
   */

  inline uint16_t jpegFlash(const Rectangle& rc,uint32_t count,uint32_t address) {
    return internal::flashBitmapOp(cmd::WRITE_FLASH_JPEG,rc,count,address);
  }


  /*
   * Write an uncompressed bitmap stored in the CoProcessor external flash IC to the display
   */

  inline uint16_t bitmapFlash(const Rectangle& rc,uint32_t count,uint32_t address) {
    return internal::flashBitmapOp(cmd::WRITE_FLASH_BITMAP,rc,count,address);
  }


  /*
   * Write a compressed bitmap stored in the CoProcessor external flash IC to the display
   */

  inline uint16_t lzgBitmapFlash(const Rectangle& rc,uint32_t count,uint32_t address) {
    return internal::flashBitmapOp(cmd::WRITE_FLASH_LZG_BITMAP,rc,count,address);
  }


  /*
   * Timer helpers
   */

  namespace internal {

    /*
     * Generic frequency setting operation for T1/T2
     */

    inline uint16_t frequencyOp(uint8_t timNumber,uint32_t period,uint16_t prescaler,ClockDivision clockDivision,CounterMode counterMode) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=cmd::TPIN_TIMER_SET_FREQUENCY;
      *ptr++=timNumber;

      *ptr++=period;
      *ptr++=period >> 8;
      *ptr++=period >> 16;
      *ptr++=period >> 24;

      *ptr++=prescaler;
      *ptr++=prescaler >> 8;

      *ptr++=clockDivision;
      *ptr++=clockDivision >> 8;

      *ptr++=counterMode;
      *ptr=counterMode >> 8;

      return 12;
    } 


    /*
     * Generic output compare init for T1/T2
     */

    inline uint16_t initCompareOp(uint8_t timNumber,uint32_t compareValue,OcMode ocMode,OcPolarity polarity,OcPreload preload) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=cmd::TPIN_TIMER_INIT_COMPARE;
      *ptr++=timNumber;

      *ptr++=compareValue;
      *ptr++=compareValue >> 8;
      *ptr++=compareValue >> 16;
      *ptr++=compareValue >> 24;

      *ptr++=ocMode;
      *ptr++=ocMode >> 8;

      *ptr++=polarity;
      *ptr++=polarity >> 8;

      *ptr++=preload;
      *ptr=preload >> 8;

      return 12;
    }


    /*
     * Generic set compare operation for T1/T2
     */
     
    inline uint16_t setCompareOp(uint8_t timNumber,uint32_t compareValue) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=cmd::TPIN_TIMER_SET_COMPARE;
      *ptr++=timNumber;

      *ptr++=compareValue;
      *ptr++=compareValue >> 8;
      *ptr++=compareValue >> 16;
      *ptr=compareValue >> 24;

      return 6;
    }
  }


  /*
   * Initialise the T1 timer
   */

  inline uint16_t t1Frequency(uint32_t period,uint16_t prescaler,ClockDivision clockDivision,CounterMode counterMode) {
    return internal::frequencyOp(1,period,prescaler,clockDivision,counterMode);
  }


  /*
   * Initialise the T2 timer
   */

  inline uint16_t t2Frequency(uint32_t period,uint16_t prescaler,ClockDivision clockDivision,CounterMode counterMode) {
    return internal::frequencyOp(2,period,prescaler,clockDivision,counterMode);
  }


  /*
   * Initialise the T1 output compare
   */

  inline uint16_t t1InitCompare(uint32_t compareValue,OcMode ocMode,OcPolarity polarity,OcPreload preload) {
    return internal::initCompareOp(1,compareValue,ocMode,polarity,preload);
  }


  /*
   * Initialise the T2 output compare
   */

  inline uint16_t t2InitCompare(uint32_t compareValue,OcMode ocMode,OcPolarity polarity,OcPreload preload) {
    return internal::initCompareOp(2,compareValue,ocMode,polarity,preload);
  }


  /*
   * Set the T1 compare value
   */

  inline uint16_t t1SetCompare(uint32_t compareValue) {
    return internal::setCompareOp(1,compareValue);
  } 


  /*
   * Set the T2 compare value
   */

  inline uint16_t t2SetCompare(uint32_t compareValue) {
    return internal::setCompareOp(2,compareValue);
  }


  /*
   * T1/T2 control operations
   */

  namespace internal {

    inline uint16_t timControlOp(uint16_t timNumber,uint8_t enable) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=cmd::TPIN_TIMER_CONTROL;
      *ptr++=timNumber;
      *ptr=enable;

      return 3;
    }
  }


  /*
   * Enable/disable T1/T2
   */

  inline uint16_t t1Enable() {
    return internal::timControlOp(1,1);
  }

  inline uint16_t t1Disable() {
    return internal::timControlOp(1,0);
  }

  inline uint16_t t2Enable() {
    return internal::timControlOp(2,1);
  }

  inline uint16_t t2Disable() {
    return internal::timControlOp(2,0);
  }


  /*
   * T1/T2 GPIO operations
   */

  namespace internal {

    inline uint16_t gpioModeOp(uint8_t pinNumber,GpioSlew slew,GpioDrive drive) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=cmd::TPIN_GPIO_CONFIGURE;
      *ptr++=pinNumber;
      *ptr++=slew;
      *ptr=drive;

      return 4;
    }

    inline uint16_t gpioControlOp(uint8_t pinNumber,uint8_t set) {

      uint8_t *ptr=CoProcessor::buffer;

      *ptr++=cmd::TPIN_GPIO_CONTROL;
      *ptr++=pinNumber;
      *ptr=set;

      return 3;
    }
  }


  /*
   * Configure GPIO for T1/T2
   */

  inline uint16_t t1Gpio(GpioSlew slew,GpioDrive drive) {
    return internal::gpioModeOp(1,slew,drive);
  }

  inline uint16_t t2Gpio(GpioSlew slew,GpioDrive drive) {
    return internal::gpioModeOp(2,slew,drive);
  }


  /*
   * Set/reset the T1/T2 GPIO pins
   */

  inline uint16_t t1GpioSet() {
    internal::gpioControlOp(1,1);
  }

  inline uint16_t t1GpioReset() {
    internal::gpioControlOp(1,0);
  }

  inline uint16_t t1GpioControl(bool set) {
    internal::gpioControlOp(1,set);
  }

  inline uint16_t t2GpioSet() {
    internal::gpioControlOp(2,1);
  }

  inline uint16_t t2GpioReset() {
    internal::gpioControlOp(2,0);
  }

  inline uint16_t t2GpioControl(bool set) {
    internal::gpioControlOp(2,set);
  }
}
