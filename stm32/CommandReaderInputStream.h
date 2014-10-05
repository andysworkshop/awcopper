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
