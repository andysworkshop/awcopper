# Andy's Workshop Graphics Coprocessor API

Welcome to the API documentation for the Andy's Workshop Graphics Coprocessor, or _awcopper_ for short. awcopper is the zero-pin, low resource, high-performance 640x360 TFT display for the Arduino Uno R3 microcontroller.

## Intended audience

This document is intended for the use of Arduino developers who want to know how to program the coprocessor.

## Download and install the Arduino library

1. Click here to download the library
2. Expand the archive into your Arduino `libraries` installation directory.

## Initialisation and Introduction

You need to declare a single instance of the co-processor for your sketch. The orientation of the display is soft-coded into the ARM coprocessor and can be changed by flashing its firmware. The default orientation is landscape (horizontal). This is how you declare the coprocessor object for a display in the landscape orientation:

    typedef awc::LandscapeCoProcessor Copper;
    Copper copro;

Similarly, a display in the portrait (vertical) orientation can be declared like this:

    typedef awc::PortraitCoProcessor Copper;
    Copper copro;

Did you notice the `awc` namespace in there? The entire library lives inside the `awc` namespace so that its simple names cannot clash with other Arduino libraries. That means you have to prefix all calls with `awc` or you can use the C++ `using` keyword to bring individual names, or even the entire library into the current scope. For example, the graphics library has these three statements at global scope:

    // bring in Rectangle, Point and Size for easy reference

    using awc::Rectangle;
    using awc::Point;
    using awc::Size;

I do this because these base objects are so commonly used that my code reads more cleanly if I don't have to prefix the use of these objects with `awc::`. You can do this too if you want, but it will only work if you don't have other libraries that make use of those same names.

Every Arduino sketch must have a `setup()` method that you can use to initialise your peripheral hardware, and we need to do the same for our coprocessor. Let's have a look at what needs to be done by reference to the `setup()` method in the graphics library example code:

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

The first two code lines initialise the Arduino I<sup>2</sup>C library, more commonly known as _Wire_. We call `Wire.begin()` to initialise _Wire_ and then I force the I<sup>2</sup>C peripheral embedded in the Arduino chip to work at 400kHz using the `TWBR=12` statement, which, under the hood boils down to an on-chip register access.

_awcopper_ will happily work at both the common I<sup>2</sup>C frequencies of 100 and 400kHz so if you already have a peripheral that must operate at either of those frequencies then that's fine, _awcopper_ doesn't care.

`copro.reset()` issues a reset command to _awcopper_ which will cause the ARM chip to execute a software reset and restart operation. The library includes a short delay after the actual reset operation to ensure the coprocessor is ready by the time your `reset()` call completes.

Next we see some commands being sent to the coprocessor and here you get your first taste of the `streaming` concept behind the whole API. 

Commands written to the coprocessor are serialised by the library, placed on the I<sup>2</sup>C bus, received by _awcopper_, queued into an on-chip FIFO and then dequeued, deserialised and executed asynchronously. This non-blocking, streaming concept lends itself very naturally to the very common C++ _output stream_ overload of the `<<` operator. The result, I'm sure you'll agree, is very clean, easy to follow code. Every _awcopper_ command can be chained to a previous command using the streaming `<<` operator using your `copro` object as the destination.

I mentioned briefly there that the commands are non-blocking, and it's true. You can stream graphics commands into the coprocessor's FIFO and it will execute them in the order in which you send them, freeing you to continue processing in your Arduino whilst the graphics are being executed by the ARM processor. 

If you manage to get far ahead of the ARM and actually fill up the FIFO then the ARM will automatically _stretch_ the I<sup>2</sup>C clock, causing your next write operation to block until space becomes available in the FIFO. This flow-control mechanism ensures that you cannot cause a buffer-overrun on the coprocessor.

## API reference

Now that you've got a clear idea of how to get started, and you're happy with the concepts behind the operation of the library, let's take a look at the operations offered by the library.

### API objects

The API makes use of several support objects that you'll often see being used in the graphical operations.

### Rectangle

    Rectangle()
    Rectangle(x,y,width,height)
    Rectangle(const Point& p,const Size& size)
    Rectangle(const Rectangle& src)

The `Rectangle` object defines a rectangular area on the display. Various constructors allow you to set up the object from the data that you have available, or you can directly access the `X`, `Y`, `Width` and `Height` members after construction.

| Member  | Description |
|---------|-------------|
| `Size getSize() const` | Get the dimensions of the rectangle as a Size object |
| `Point getTopLeft() const` | Get the location of the top-left point of the rectangle as a Point object |
| `int16_t Right() const` | Get the X co-ordinate of the right-hand edge |
| `int16_t Bottom() const` | Get the Y co-ordinate of the lower edge |

### Point

    Point()
    Point(int16_t x,int16_t y)
    Point(const Point& p)

Represents a single point on the display. The default constructor initialise the `X` and `Y` members to `(0,0)`.

| Member  | Description |
|---------|-------------|
| `static const Point Origin` | `Point::Origin` is a convenient and readable way to get a Point object set to (0,0) |

### Size

    Size()
    Size(int16_t width,int16_t height)
    Size(const Size& src)

Represents the combination of a width and a height. The default constructor initialises the `Width` and `Height` members to zero, or you can change them manually after construction.

| Member  | Description |
|---------|-------------|
| `uint32_t getArea() const` | Convenience method to return the product of the width and height as a 32-bit value. |

### Gamma

    Gamma(const uint8_t *levels) {

The gamma object is a wrapper for the array of 13 bytes that apply gamma correction to the panel to compensate for variations in the panel's performance. The default constructor takes a copy of the 13 values and then the `Gamma` object can be passed to the `gamma()` function that applies it to the panel.

| Member  | Description |
|---------|-------------|
| `static Gamma getDefault()` | This method will get you a Gamma object that is preset to the values that I have, by experimentation, found to be useful for the clone Sony Vivaz U5 panels that you can get on ebay. |

### Bytes

    Bytes(const void *ptr,uint32_t count)

If you're going to be using the _awcopper_ API to display graphics images then you'll need to stream those bytes into the API and that's handled by wrapping them in a `Bytes` structure. You'll see examples of this in the API documentation for the image operations. For now, just know that you can use the simple constructor to describe the details of a byte buffer. A copy of bytes is not taken so make sure they remain in scope until you've passed them to an _awcopper_ API function.

### Colour names

    namespace awc {
      enum {
        SNOW=0xfffafa,
        GHOSTWHITE=0xf8f8ff,
        WHITESMOKE=0xf5f5f5,
        GAINSBORO=0xdcdcdc,

        [ and a whole lot more ...]   

Whenever an API operation takes a colour parameter you can specify directly as a hex RGB value in the form `0x00rrggbb` or, if you prefer, you can refer to it by name using one of the names in the `ColourNames.h` file. For example, `awc::WHITE`. It's more readable that way. The set of colour names are known as the _X11 colour names_. See the [Wikipedia page](http://en.wikipedia.org/wiki/X11_color_names) for a full list with associated swatches.

Be aware that because the display is operating in 16-bit mode then the actual colour fidelity is lower than the full 8 bits-per-pixel implied by the 24-bit hex format.

### Useful constants

Way back when we were initialising we used a `typedef` to define the display orientation, like this: `typedef awc::PortraitCoProcessor Copper`. Now that we've got a concise `Copper` type we can use it to refer to some useful constants:

| Constant | Value |
|-|-|
| `Copper::WIDTH` | The display width. 640, or 360 for landscape or portrait mode, respectively |
| `Copper::HEIGHT` | The display height. 360, or 640 for landscape or portrait mode, respectively |

### API functions to change the colours

The API uses the concept of _foreground_ and _background_ colours.  Many of the operations that draw something on the screen use one of these colours. Let's see two functions that change the colours.

### foreground()

    foreground(uint32_t colorref)

This function sets the current foreground colour to `colorref`.

    copro << awc::foreground(awc::BLACK);

An example showing how to set the current foreground colour to black.

### background()

    background(uint32_t colorref)

This function sets the current background colour to `colorref`.

    copro << awc::background(awc::BLUE);

An example showing how to set the current background colour to blue.

### Graphical API operations

Now it's time to take a look at the graphical operations that allow you to draw on the display. Points, rectangles, ellipses and gradients are all covered here.

### clearRectangle()

    clearRectangle(const Rectangle& rc)
   
This function clears (fills) a rectangle with the current background colour.

    copro << awc::background(awc::RED)
          << awc::clearRectangle(Rectangle(0,0,100,200));

The example will fill a rectangle 100 pixels wide and 200 high located at the origin with red pixels.

### fillRectangle()

    fillRectangle(const Rectangle& rc)

This function will fill a rectangle with the current foreground colour.

    Rectangle rc(0,0,100,200);
    copro << awc::foreground(awc::GREEN)
          << awc::fillRectangle(rc);

The example will fill a rectangle 100 pixels wide and 200 high located at the origin with green pixels.

### rectangle()

    rectangle(const Rectangle& rc)

A function that allows you to draw a rectangle with a single pixel outline with the current foreground colour.

    copro << awc::foreground(awc::WHITE)
          << awc::rectangle(Rectangle(0,0,Copper::WIDTH,Copper::HEIGHT));

This example shows how to draw a white rectangle that is the full width and height of the display.

### clear()

    clear()

Not much to this one. It's a shortcut that clears down the entire display to the current background colour.

    copro << awc::clear();

### gradientFillRectangle()

    gradientFillRectangle(const Rectangle& rc,
                          Direction dir,
                          uint32_t firstColour,
                          uint32_t lastColour)

This function performs a linear gradient fill of a rectangular region. The gradient can run horizontally (left-to-right) or vertically (top-to-bottom). You specify the starting and ending colours and _awcopper_ computes the step changes that it needs to make to transition between the two.

The `Direction` type is an enumeration containing the values  `HORIZONTAL` and `VERTICAL`.

    Rectangle rc(0,0,Copper::WIDTH,Copper::HEIGHT);
    copro << awc::gradientFillRectangle(rc,Direction::HORIZONTAL,awc::BLACK,awc::RED);

This example will fill the whole display with a horizontal gradient that starts at black and fades into full intensity red.

### plot()

    plot(const Point& p)

A simple method that allows you to plot a single point on the display in the foreground colour.

    copro << awc::foreground(awc::BLUE)
          << awc::plot(250,250);

The example will plot a blue point at `(250,250)` on the display.

### ellipse()

    ellipse(const Point& center,const Size& size)

Allows you to draw an ellipse with a single pixel border in the foreground colour. The ellipse will be centered at the `center` parameter and have radii defined by the `Width` and `Height` members of the `size` parameter.

    Point p(Copper::WIDTH/2,Copper::HEIGHT/2);
    Size s(100,100);
    copro << ellipse(p,s);

This example will draw a circle with a radius of 100 pixels in the center of the display, using the current foreground colour.

### fillEllipse()

    fillEllipse(const Point& center,const Size& size)

Allows you to draw an ellipse filled with the current foreground colour. The ellipse will be centered at the `center` parameter and have radii defined by the `Width` and `Height` members of the `size` parameter.

    copro << foreground(awc::WHITE)
          << fillEllipse(Point(200,200),Size(50,75));

This example will fill a white ellipse at `(200,200)`. The ellipse will have a horizontal radius of 50 pixels and a vertical radius of 75 pixels.

### line()

    line(const Point& p1,const Point& p2)

Allows you to draw a line with a width of 1 pixel that connects the two points. The line will be drawn with the current foreground colour.

     copro << awc::foreground(random(awc::WHITE))
           << awc::line(Point::Origin,Point(100,100));

The example draws a line from the origin to `(100,100)` in a randomly chosen colour. The Arduino library `random()` function is used to select a random number between 0 and `awc::WHITE`, which is `0x00ffffff`, the maximum value that a colour can have.

### polyline()

    polyline(const Point *p,uint8_t count)

The `polyline` method allows you to draw a sequence of lines that connect the array of points that you supply. The lines will be drawn in the current foreground colour.

    Point p[3];

    p[0]=Point::Origin;
    p[1].X=Copper::WIDTH-1;
    p[1].Y=0;
    p[2].X=Copper::WIDTH-1;
    p[2].Y=Copper::HEIGHT-1;

    copro << awc::polyline(p,sizeof(p)/sizeof(p[0]));

This example will draw a line from the origin at the top-left across to the top-right corner and then down to the bottom-right corner. The line will be drawn in the currently selected foreground colour.

### font()

    font(FontId fid)

This function sets the font that will be used by the text output methods. _awcopper_ comes shipped with nine preset fonts that you can choose from that have a variety of sizes and styles. All of these were downloaded from the [dafont](http://www.dafont.com) website.

The `FontId` type is an enumeration that can take one of the following values:

| FontId | Name | Pixel Height |
|-|-|-|
| `awc::APPLE` | [Apple](http://www.dafont.com/apple.font) | 8 |
| `awc::ATARI` | [Atari ST](http://www.dafont.com/atari-st-8x16-system-font.font) | 16 |
| `awc::DOS` | [Perfect DOS VGA](http://www.dafont.com/perfect-dos-vga-437.font) | 8 |
| `awc::KYROU_BOLD` | [Grixel Kyrou 9 bold](http://www.dafont.com/grixel-kyrou-9.font) | 8 |
| `awc::KYROU` | [Grixel Kyrou 9](http://www.dafont.com/grixel-kyrou-9.font)| 8 |
| `awc::NINTENDO` | [Nintendo DS BIOS](http://www.dafont.com/nintendo-ds-bios.font) | 16 |
| `awc::PIXELADE` | [Pixelade](http://www.dafont.com/pixelade.font) | 13 |
| `awc::PROGGY` | [Proggy](http://www.dafont.com/proggy-clean.font) | 16 |
| `awc::GOLDFISH` | [Volter (Goldfish)](http://www.dafont.com/volter-goldfish.font) | 9 |

Click on the links in the table to see a preview of each font. Due to the high pixel density of the U5 Vivaz display you may find that the 16-pixel fonts are more readable for general purpose use.

    copro << awc::font(awc::ATARI);

Set's the current font to `awc::ATARI`.

### text()

    text(const Point& p,const char *str,TextMode textMode=TRANSPARENT)

This function allows you to draw some text on the display at the position you supply in the foreground colour. `TextMode` is an enumeration that contains the values `awc::TRANSPARENT` and `awc::SOLID`.  If you chose _transparent_ then the gaps between the glyphs that make up each character will be skipped over so that the background will show through. Conversely, _solid_ mode will fill the gaps between the character glyphs with the current background colour.

    copro << awc::foreground(awc::WHITE)
          << awc::font(awc::ATARI)
          << awc::text(Point::Origin,"Hello World");

This sequence will set the current foreground to white, the font to _Atari ST_ and then draw the text "Hello World" at the origin. I have chosen accept the default argument of `awc::TRANSPARENT` instead of specifying it manually. The code reads cleaner that way.

### The Image API

_awcopper_ offers a comprehensive suite of functionality for displaying bitmapped images. JPEG, LZG (it's like PNG) and uncompressed bitmaps are all supported and you can just to stream them from your Arduino or from the on-board flash chip supplied with _awcopper_.


#### Uncompressed images

Uncompressed images are nothing more than a stream of pixel data pre-rendered into a form that can be streamed directly into the display. The _bm2rgbi_ tool supplied with _awcoppper_ will create these uncompressed files for you from a variety of sources such as JPEG, PNG etc.

    bm2rgbi.exe test.jpg test.bin r61523 64

`bm2rgbi` takes four mandatory parameters, the first two of which are important to you. The first parameter is the filename of the image to convert. All the common web bitmap formats are supported such as JPEG and PNG.

The second parameter is the name of the output file. The fourth and fifth parameters, `r61523 64` are constant. `bm2rgbi` is capable of converting graphics to many types of panel and colour depth. We are only interested in the _awcopper_ panel driver and that is a Renesas R61523 running in 64K colour mode.

The primary advantage of uncompressed images is that they are very fast because there is no decompression required. Pair this with a fast storage medium such as the on-board flash and you'll get the best performance. The downside is that uncompressed images can get very large. You wouldn't want to try storing too many of them in your Arduino Uno program memory for example.

#### LZG Compressed images

LZG is a compact compressor that offers very similar performance to the PNG format with less file format overhead. If your graphics are primarily computer generated, such as icons, then LZG will probably get you the best compression. The _bm2rgbi_ tool supplied with _awcoppper_ will create LZG-compressed files for you from a variety of sources such as JPEG, PNG etc.

    bm2rgbi.exe test.jpg test.bin r61523 64 -c

The example is very similar to what we used to create an uncompressed bitmap except that we have added the `-c` option that tells `bm2rgbi` to compress the output file using the LZG compressor.

The only downside to LZG compressed images is that there is a processing overhead associated with the decompression. This overhead may, however, be negated by the advantage of having to transfer less data from the storage medium.

#### JPEG Compressed images

Everyone knows JPEGs, right? They offer tunable compression and great ratios when the image is more 'real world' than computer generated, for example with photographs. _awcopper_ supports the most common JPEG variants but does not support _progressive_ JPEGs.

JPEG decoding takes the most out of the processing power offered by the ARM coprocessor so you'll notice the image appearing on screen as it's been decoded.

### Image API functions

Let's take a look at the functions offered by the image API.

### jpeg()

    jpeg(const Rectangle& rc,uint32_t count)

The `jpeg` method begins the process of writing a JPEG image to the display. The `rc` parameter defines where on the display you want it to appear. The `Width` and `Height` members of the rectangle must exactly match the dimensions of the JPEG. The `count` parameter is the number of bytes in the JPEG file.

This method tells _awcopper_ to expect the bytes that make up the JPEG to be sent to it immediately afterwards. You cannot issue any other command (except `reset()`) until those bytes have been sent. How do you send the data? Easy, you just stream as many `Bytes` structures as necessary into the coprocessor object as you need to.

    uint16_t size,batchSize,i;
    const uint8_t *data;
    uint8_t buffer[128],*bufptr;   // multiple of 32 makes the most of the Wire buffer
    
    data=reinterpret_cast<const uint8_t *>(&JpegData);
    size=reinterpret_cast<uint16_t>(&JpegDataSize);

    copro << awc::jpeg(Rectangle(70,26,500,308),size);

    // send the expected data in batches

    for(;size;size-=batchSize) {
    
      batchSize=size<sizeof(buffer) ? size : sizeof(buffer);
      bufptr=buffer;
    
      for(i=batchSize;i;i--)
        *bufptr++=pgm_read_byte_near(data++);
    
      copro << awc::Bytes(buffer,batchSize);
    }

This example is taken from the JPEG demo in the _GraphicsMethods_ example sketch. You can see how we first use the `jpeg()` function to tell _awcopper_ whats coming next and then we go into a loop sending the data in small batches.

If you can fit your images along with your program into the 32Kb space on the Arduino then it's possible to include the image data directly, and that's what the example above does. Take a look at the _GraphicsMethods_ example and in particular the `JpegGraphics.cpp` tab to see how we use a trivially small and reusable bit of assembly language to directly import binary data into our sketch.

### bitmap()

    bitmap(const Rectangle& rc,uint32_t count)

The `bitmap()` method is used to tell _awcopper_ that we want to display an uncompressed bitmap. This merely prepares _awcopper_ to receive the data and tells it to expect a stream of exactly `count` bytes.

The way that this streaming works is exactly the same as for the `jpeg()` method, documented above. Please see the above documentation for `jpeg()` for reference and the _UncompressedBitmap_ example code for an example.

### lzgBitmap()

    lzgBitmap(const Rectangle& rc,uint32_t count)

The `lzgBitmap()` method is used to tell _awcopper_ that we want to display an LZG compressed bitmap. This merely prepares _awcopper_ to receive the data and tells it to expect a stream of exactly `count` compressed bytes.

The way that this streaming works is exactly the same as for the `jpeg()` method, documented above. Please see the above documentation for `jpeg()` for reference and the _LZGBitmap_ example code for an example.

### The onboard flash chip

_awcopper_ comes with an onboard 16 megabit flash IC that allows you to store graphics for fast retrieval while your program is running. The _awcopper_ API offers full access to the program/erase operations and a _ProgramFlash_ example is supplied that allows you to download images from your PC over the USB cable and program them into flash.

### eraseFlash()

    eraseFlash()

Sectors in the flash device must be erased before they can be programmed. The `eraseFlash()` method erases the entire flash IC. This will take some seconds to execute. During that time you may continue to stream more commands to _awcopper_ and they will be queued in the FIFO for execution as soon as the erase command has completed.

    copro << awc::eraseFlash();

Simple to execute, but it will take a few seconds to complete. The blue _busy_ LED will go out when the command has completed.

### erase4KSector()

    erase4KSector(uint32_t address)

Flash devices tend to be arranged into rather large blocks, 64Kb in the case of the _awcopper_ 16Mb chip. It wastes space and causes un-necessary wear on the flash memory if you have to erase 64Kb when you only want to write a smaller amount. This flash IC allows you to erase smaller sectors within the blocks.

`erase4KSector()` will erase 4096 bytes starting at the `address` that you supply. The address is a byte address within the device that must be aligned to a 4096 byte boundary.

    copro << awc::erase4KSector(8192);

The example will erase 4096 bytes starting at address 8192 (the third sector).

### erase8KSector()

    erase8KSector(uint32_t address)

Just like `erase4KSector()` except that it deals in 8192 byte sectors.

### erase64KSector()

    erase64KSector(uint32_t address)

Just like `erase4KSector()` except that it deals in 65536 byte sectors, a complete block.

### program()

    program(uint32_t address)

Use the `program()` function to begin writing a page of data to the flash device at the location given by `address`, which must be on a 256 byte boundary. A page consists of exactly 256 bytes.

`program()` tells _awcopper_ to expect your 256 bytes to arrive next. _awcopper_ will buffer bytes that arrive until exactly 256 have been sent. It will then write your page of data to the flash device. Bytes are sent to the coprocessor by streaming in `Bytes` structures in exactly the same way as you do when you are streaming data into one of the image functions.

The page must have been erased using one of the erase functions before it can be programmed.

    uint8_t flashPage[256];
    
    // populate flashPage with valid data
    
    copro << awc::program(8192) 
          << awc::Bytes(flashPage,256);

The example shows the basic page-program sequence. You don't need to worry about waiting for each page program to complete. Just stream in your page programming commands and data and _awcopper_ will apply its flow control to the stream if you manage to fill the FIFO.

### jpegFlash()

    jpegFlash(const Rectangle& rc,uint32_t count,uint32_t address)

The `jpegFlash` function is used to transfer a JPEG stored in the onboard flash chip to the display. The `rc` parameter defines the area on-screen where you want to display the image. The `Width` and `Height` members of the rectangle must exactly match the dimensions of the JPEG. The `count` parameter is the number of bytes in the JPEG file. The `address` parameter is the address in the flash device where the image is stored.

The flash IC is much larger than the program memory on your Arduino and can be accessed by the ARM coprocessor, asynchronously to your Arduino, at tens of megabits per second so it makes a lot of sense to store your graphics on the flash chip.

    enum {
      JPEG_ADDRESS=0,
      JPEG_SIZE=97548
    };

    const Rectangle fullScreen(0,0,Copper::WIDTH,Copper::HEIGHT);
    copro << awc::jpegFlash(fullScreen,JPEG_SIZE,JPEG_ADDRESS);

The example is taken from the _FlashBitmaps_ example and shows how you ask _awcopper_ to display a full-screen JPEG stored at location 0 with size 97548 bytes.

### bitmapFlash()

    bitmapFlash(const Rectangle& rc,uint32_t count,uint32_t address)

The `bitmapFlash` function is used to transfer an uncompressed graphic stored in the onboard flash chip to the display. The `rc` parameter defines the area on-screen where you want to display the image. The `Width` and `Height` members of the rectangle must exactly match the dimensions of the JPEG. The `count` parameter is the number of bytes in the bitmap file. The `address` parameter is the address in the flash device where the image is stored.

The ARM coprocessor is able to access uncompressed graphics from the flash chip so quickly that this method of storage is generally suitable for a responsive graphical user interface. In fact, that is what I did when developing the user interface to my [reflow oven project](http://andybrown.me.uk/wk/2014/05/11/awreflow/).

    enum {
      BITMAP_ADDRESS=97792,
      BITMAP_SIZE=460800
    };

    const Rectangle fullScreen(0,0,Copper::WIDTH,Copper::HEIGHT);
    copro << awc::bitmapFlash(fullScreen,BITMAP_SIZE,BITMAP_ADDRESS);

The example is taken from the _FlashBitmaps_ example and shows how you ask _awcopper_ to display a full-screen bitmap stored at location 97792 with size 460800 (width * height * 2) bytes.

### lzgBitmapFlash()

    lzgBitmapFlash(const Rectangle& rc,uint32_t count,uint32_t address);

The `lzgBitmapFlash` function is exactly the same as the `bitmapFlash()` function except that it expects the data stored at `address` to be LZG-compressed. Please refer to the documentation for `bitmapFlash()` for further information.

    enum {
      LZG_ADDRESS=558592,
      LZG_SIZE=255682
    };

    const Rectangle fullScreen(0,0,Copper::WIDTH,Copper::HEIGHT);
    copro << awc::lzgBitmapFlash(fullScreen,LZG_SIZE,LZG_ADDRESS);

The example is taken from the _FlashBitmaps_ example and shows how you ask _awcopper_ to display a full-screen LZG bitmap stored at location 558592 with size 255682 bytes.

### Non-graphical commands

These commands are display-related but don't actually draw anything on the display.

### backlight()

    backlight(uint8_t percentage)

The backlight function is used to set the intensity of the LED-based backlight built in to the display. `percentage` is the value that you'd like to set, between 0 and 100.

The backlight will be ramped up or down smoothly to the selected intensity. This change will take a few milliseconds so you may want to apply a delay while this happens before you start drawing.

    copro << awc::backlight(100);  

The example sets the backlight to 100%.

### sleep()

    sleep()

This command puts the display into low-power sleep mode. The backlight will be turned off abruptly so you may want to use the `backlight()` function to smoothly switch off the backlight before calling `sleep()`

    copro << awc::sleep();

### wake()

    wake()

This is the inverse of the `sleep()` command. `wake()` will bring the display back to life.

    copro << awc::wake();

### gamma()

    gamma(const Gamma& gamma)

This function applies a set of gamma correction values in the `Gamma` structure to the display. See documentation for `Gamma` structure for details.

Gamma correction allows you to compensate differences in panels from different manufacturers by applying adjustments to the way in which colours are displayed.

    copro << awc::gamma(awc::Gamma::getDefault());

The example applies a default set of gamma values to the panel. The default values were created by me after experimenting with the most popular clone panel that is available on ebay.

### window()

    window(const Rectangle& rc)

This is an advanced function that allows you to manipulate the display _window_ which is the area on the screen where graphics operations will take place. Typically you would do this before issuing `beginWriting()` and `writeData` commands. Using these methods requires knowledge of how the R61523 works and is not generally required.

    copro << awc::window(Rectangle(0,0,Copper::WIDTH,Copper::HEIGHT));

The example sets the display window to the full screen.

### beginWriting()

    beginWriting()

This advanced function is used to tell the display that you are about to send raw data to it. Normally you would have used the `window()`function immediately before this is called.

    copro << awc::beginWriting();

### writeData()

    writeData(const void *data,uint16_t count)

This advanced function is used to write a block of raw data directly to the display. `beginWriting()` must have been called before you start calling `writeData()` and you can call `writeData()` as many times as you like. There is no termination function, you can just call other functions in this API as soon as you've written your last block of raw data. `data` is a pointer to the buffer containing the raw bytes and `count` is the number of bytes in the buffer.

    unsigned int buffer[200];
    
    copro << awc::window(Rectangle(0,0,10,10))
          << awc::beginWriting()
          << awc::writeData(buffer,200);

The example shows the complete sequence of setting a window, starting a write and then writing the data.

### The T1 and T2 pins

_awcopper_ features a pair of general purpose output pins that you can program for your own needs. These pins may be programmed for GPIO or used as outputs from two of the ARM timer peripherals, for example for generating PWM waveforms.

T1 and T2 have an output high level of 2.8V though this can be changed for the GPIO modes by setting the pins to open-drain and adding an external pull-up resistor to your desired level. Let's take a look at the functionality on offer.

In all the following documentation I will use an 'X' where a timer pin number is required. For example, `tXGpio` may refer either to the `t1Gpio` or `t2Gpio` functions.

### tXGpio()

    tXGpio(GpioSlew slew,GpioDrive drive)

This function initialises a pin for GPIO. A pin may be used for GPIO or timer output but not both at the same time. `GpioSlew` is an enumeration in the `awc` namespace containing the following values:

    enum GpioSlew {
      GPIOSLEW_2 = 0,                 // 2MHz slew rate
      GPIOSLEW_10 = 1,                // 10MHz slew rate
      GPIOSLEW_50 = 36                // 50MHz slew rate
    };

The selected values controls the internal speed with which the pin changes from one level to another. Faster speeds result in a more abrupt change but can cause issues with under and overshoot. Given that you can only control these pins at the rate of the I<sup>2</sup>C bus it makes sense to favour the lower speeds and have a better signal quality.

`GpioDrive` is another enumeration that controls how the ARM device drives the GPIO pin:

    enum GpioDrive {
      GPIODRIVE_PUSHPULL = 0,         // normal 2.8V/GND driver
      GPIODRIVE_OPENDRAIN = 1,        // GND driver, open-drain high. must be pulled up externally.
    };

In push-pull mode the pin is driven both high and low by the ARM MCU. In open-drain mode the MCU drives the pin low but cannot drive it high. You must supply an external pull-up resistor to the level you desire. This mode allows you to drive peripherals whose input-high level does match the 2.8V level of _awcopper_. It also allows you to safely use a shared bus where the possibility of multiple simultaneous drivers could cause actual device damage.

    copro << awc::t1Gpio(awc::GPIOSLEW_2,awc::GPIODRIVE_PUSHPULL);

The example initialises T1 with a 2MHz slew rate in push-pull mode.

### tXGpioSet(), tXGpioReset()

    tXGpioSet()
    tXGpioReset()

These functions set (high) or reset (low) a T1/2 pin that must have been initialised with `tXGpio`.

    copro << awc::t2GpioSet();

Set's the T2 pin high (2.8V) for push-pull or open for open-drain.

### tXGpioControl()

    tXGpioControl(bool set)

Set's the T1/T2 pin to the value of the `set` parameter. `true` = high, `false` = low.

    copro << awc::t1GpioControl(false);

Sets T1 to the low state.

### txFrequency()

    txFrequency(uint32_t period,
                uint16_t prescaler,
                ClockDivision clockDivision,
                CounterMode counterMode)

This function is used to initialise a pin in timer mode. T1 and T2 run at an internal frequency equal to the MCU core clock which is officially 48MHz but could be 64MHz if you're using _awcopper_ in overclocked mode. 

`period` is the number of clock ticks after which the timer will change direction or auto-reload. If `counterMode` is `awc::COUNTERMODE_UP` then the counter will count up to `period` and then reset to zero and start again. If `counterMode` is `awc::COUNTERMODE_UP` then the counter will start at `period`and count down to zero before auto-reloading with `period`.  There are also three center-aligned modes, with `awc::COUNTERMODE_CENTER3` being the one that counts up then reverses back to zero, then counts up again, and so on.

`prescaler` is the number by which the clock should be divided, minus one. So to divide by 2 `prescaler` should be 1.

`clockDivision` is yet another clock divider that can be set to `awc::CKDIV_1`,  `awc::CKDIV_2` or `awc::CKDIV_4` to further divide down the clock to get lower frequencies.

      copro << awc::t1Frequency(           // use T1 in timer mode          
                  1999,                    // auto-reload at 2000
                  1,                       // prescaler of 2 (24/32MHz)
                  awc::CKDIV_1,            // no clock-division
                  awc::COUNTERMODE_UP)     // timer counts up

The example sets the timer to count upwards until it hits 2000 when it will reset to zero and start counting up again. Assuming an overclocked 64MHz MCU then the timer clock ticks at 64/2/1 = 32MHz. There are 2000 ticks so it will hit the reload value of 2000 at a frequency of 32MHz/2000 = 16kHz.

What we've done here is to prepare the timer to do _something_ at a frequency of 16kHz. As we'll see next we can toggle our output pins to create fixed frequency pulses or PWM waveforms.