/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


/*
 * Implementation of an InputStream that gets data from the command reader I2C circular buffer.
 * This is designed specifically to read data for JPEG images and such it does not bother to
 * implement methods that will never get called.
 */

class CommandReaderInputStream : public InputStream {

  protected:
    ManagedCircularBuffer& _cbuffer;
    uint32_t _remaining;

  public:
    CommandReaderInputStream(ManagedCircularBuffer& cbuffer,uint32_t remaining);

  public:

    // overrides from InputStream

    virtual int16_t read() override;
    virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead) override;
    virtual bool skip(uint32_t howMuch) override;
    virtual bool available() override;
    virtual bool reset() override;

    // overrides from StreamBase

    virtual bool close() override;
};


/*
 * Constructor
 */

inline CommandReaderInputStream::CommandReaderInputStream(ManagedCircularBuffer& cbuffer,uint32_t remaining)
  : _cbuffer(cbuffer),
    _remaining(remaining) {
}


/*
 * Read a single byte: blocking
 */

inline int16_t CommandReaderInputStream::read() {

  // check for end

  if(_remaining==0)
    return E_END_OF_STREAM;

  // decrease remaining and return data

  _remaining--;
  return _cbuffer.managedRead();
}


/*
 * Read a buffer of data
 */

inline bool CommandReaderInputStream::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {

  // check for the end

  if(_remaining==0) {
    actuallyRead=0;
    return true;
  }

  // can only read up to the maximum desired

  actuallyRead=size>_remaining ? _remaining : size;

  _remaining-=actuallyRead;
  _cbuffer.managedRead(reinterpret_cast<uint8_t *>(buffer),actuallyRead);

  return true;
}


/*
 * Return amount available
 */

inline bool CommandReaderInputStream::available() {
  return _cbuffer.availableToRead()>0;
}


/*
 * Skip forwards (LZG dcompressor uses this)
 */

inline bool CommandReaderInputStream::skip(uint32_t howMuch) {

  while(howMuch--)
    read();

  return true;
}


/*
 * These methods will never get called
 */

inline bool CommandReaderInputStream::reset() {
  return false;
}


inline bool CommandReaderInputStream::close() {
  return false;
}
