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

#### Rectangle

	Rectangle()
    Rectangle(x,y,width,height)
    Rectangle(const Point& p,const Size& size)
	Rectangle(const Rectangle& src)

The `Rectangle` object defines a rectangular area on the display. Various constructors allow you to set up the object from the data that you have available, or you can directly access the `X`, `Y`, `Width` and `Height` members after construction.

<table>
<tr><th>Member</th><th>Description</th></tr>
<tr><td>Size getSize() const</td><td>Get the dimensions of the rectangle as a Size object</td></tr>
<tr><td>Point getTopLeft() const</td><td>Get the location of the top-left point of the rectangle as a Point object</td></tr>
<tr><td>int16_t Right() const</td><td>Get the X co-ordinate of the right-hand edge</td></tr>
<tr><td>int16_t Bottom() const</td><td>Get the Y co-ordinate of the lower edge</td></tr>
</table>

#### Point

	Point()
	Point(int16_t x,int16_t y)
	Point(const Point& p)

Represents a single point on the display. The default constructor initialise the `X` and `Y` members to `(0,0)`.

<table>
<tr><th>Member</th><th>Description</th></tr>
<tr><td>static const Point Origin</td><td>Point::Origin is a convenient and readable way to get a Point object set to (0,0)</</table>

### Non-graphical operations

This section will describe the operations available that do not directly draw anything on the screen.

