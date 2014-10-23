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

### Image API operations