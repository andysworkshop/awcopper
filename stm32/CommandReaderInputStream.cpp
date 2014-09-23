/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#include "Application.h"


/*
 * Read a single byte: blocking
 */

int16_t CommandReaderInputStream::read() {

  // check for end

  if(_remaining==0)
    return E_END_OF_STREAM;

  // block until available

  while(!available());

  // decrease remaining and return data

  _remaining--;
  return _cbuffer.read();
}


/*
 * Read a buffer of data
 */

bool CommandReaderInputStream::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {

  uint32_t available;

  // check for the end

  if(_remaining==0) {
    actuallyRead=0;
    return true;
  }

  // block until something is available

  while((available=_cbuffer.availableToRead())==0);

  // can only read up to the maximum desired

  actuallyRead=available>size ? size : available;

  _remaining-=actuallyRead;
  _cbuffer.read(reinterpret_cast<uint8_t *>(buffer),actuallyRead);

  return true;
}


/*
 * Return amount available
 */

bool CommandReaderInputStream::available() {
  return _cbuffer.availableToRead()>0;
}


/*
 * Skip forwards (LZG dcompressor uses this)
 */

bool CommandReaderInputStream::skip(uint32_t howMuch) {

  while(howMuch--)
    read();

  return true;
}


/*
 * These methods will never get called
 */

bool CommandReaderInputStream::reset() {
  return false;
}
bool CommandReaderInputStream::close() {
  return false;
}
