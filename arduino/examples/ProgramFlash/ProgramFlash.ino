/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 *
 * This application will wait continually for data to be sent to it
 * on the serial port. It will program those data pages to the flash
 * chip. The framing of the data is a continuous stream of the following
 * frames:
 * 
 * 0..9:    preamble: 0x55, 0xaa, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
 * 10..12:  24-bit address in flash, LSB first.
 * 13..268: page data. 256 bytes.
 * 269:     checksum calculated as XOR of all bytes in 256-byte page.
 */
 
#include <Wire.h>
#include <awcopper.h>

// declare an instance of the coprocessor

typedef awc::LandscapeCoProcessor Copper;
Copper copro;


// current output line

static int16_t textLine;


void prompt(const char *str,uint32_t cr=awc::WHITE);


// response bytes to the peer

namespace ResponseCodes {     // encapsulate these dangerously common names
  enum {
    NACK = 0,
    ACK  = 1
  };
}


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
        << awc::font(awc::DOS)                      // select the DOS font
        << awc::backlight(100);                     // bring backlight to 100%

  // the backlight has a smooth rising curve, wait for it
  
  delay(500);

  // initialise the serial port at 115K

  Serial.begin(115200);

  // start writing at the origin (first prompt will add 16)

  textLine=-16;
}


/*
 * Main loop
 */

void loop() {
  
  uint8_t page[256],pageChecksum;
  uint32_t address;
  
  waitPreamble();
  readAddress(address);
  readPage(page,pageChecksum);
  
  if(!readChecksum(pageChecksum))
    sendNack();
  else {
    programPage(address,page);
    sendAck();
  }
}


/*
 * wait for the preamble to arrive 
 */

void waitPreamble() {

  uint8_t c,pos;

  // expected sequence to start a page program

  static const uint8_t preamble[] = { 
    0x55, 0xaa, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
  };

  // wait for the bytes to arrive

  prompt("Waiting for preamble...");

  pos=0;
  while(pos<sizeof(preamble)/sizeof(preamble[0])) {

    // get next byte

    while(!Serial.available());
    c=Serial.read();

    // error in preamble

    if(c!=preamble[pos]) {
      errorPrompt("Invalid preamble byte received, restarting");
      pos=0;
    }
    else
      pos++;
  }
}


/*
 * Read the 24-bit address from the peer
 */

void readAddress(uint32_t& address) {

  uint8_t i,c;

  prompt("Reading address in flash");

  address=0;

  for(i=0;i<3;i++) {

    while(!Serial.available());
    c=Serial.read();

    address<<=8;
    address|=c;
  }
}


/*
 * Read the 256 byte page
 */

void readPage(uint8_t *flashPage,uint8_t& pageChecksum) {

  uint8_t received,c;

  prompt("Waiting for 256 bytes of flash page data");

  received=0;
  pageChecksum=0;

  do {

    // wait for byte and add to page

    while(!Serial.available());
    c=Serial.read();

    flashPage[received++]=c;
    pageChecksum^=c;

  } while(received!=0);  // done when wrapped
}


/*
 * Read and validate the single byte checksum
 */

bool readChecksum(uint8_t& ourChecksum) {

  uint8_t theirChecksum;

  prompt("Reading checksum");

  while(!Serial.available());
  theirChecksum=Serial.read();

  if(theirChecksum!=ourChecksum) {
    errorPrompt("Failed checksum, cannot program this page");
    return false;
  }

  return true;
}


/*
 * Program the flash page
 */

void programPage(uint32_t address,const uint8_t *flashPage) {

  // we can stream in as many of these requests as the coprocessor can buffer
  // it'll block back via the I2C bus if/when we fill the FIFO

  copro << awc::program(address) 
        << awc::Bytes(flashPage,256);
}


/*
 * Send a NACK code back to the peer
 */

void sendNack() {
  prompt("Sending NACK to peer");
  Serial.write(ResponseCodes::NACK);
}


/*
 * Send an ACK code back to the peer
 */

void sendAck() {
  prompt("Sending ACK to peer");
  Serial.write(ResponseCodes::ACK);
}


/*
 * Print an error (red text)
 */

void errorPrompt(const char *str) {
  prompt(str,awc::RED);
}


/*
 * Print a text prompt
 */

void prompt(const char *str,uint32_t cr) {

  // move to the next line, or the first if this is the first caller

  textLine+=16;
  if(textLine>=Copper::HEIGHT) {
    copro << awc::clear();
    textLine=0;
  }

  // set the foreground colour and write out the string

  copro << awc::foreground(cr)
        << awc::text(awc::Point(0,textLine),str);
}

