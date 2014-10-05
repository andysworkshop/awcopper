/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


/*
 * The managed circular buffer adds IRQ suspend/resume functionality to an ordinary
 * circular buffer. The base class readXXX() methods should not be used. Any read
 * method potentially un-suspends the IRQ.
 */

class ManagedCircularBuffer : private circular_buffer<uint8_t> {

  protected:
    volatile bool _suspended;

  protected:
    void checkSuspended() volatile;

  public:
    ManagedCircularBuffer(uint32_t size);

    // replacements for the base class read() functions. Renamed to make it
    // more obvious that the call is going to this managed wrapper

    void managedRead(uint8_t *output,uint32_t size) volatile;
    uint8_t managedRead() volatile;

    void suspend();       // suspend (disable IRQ)

    // inheritance is private to hide the read() methods so we explicity allow
    // these safe methods

    using circular_buffer<uint8_t>::availableToRead;
    using circular_buffer<uint8_t>::availableToWrite;
    using circular_buffer<uint8_t>::write;
};


/*
 * Constructor
 */

inline ManagedCircularBuffer::ManagedCircularBuffer(uint32_t size)
  : circular_buffer<uint8_t>(size) {
}


/*
 * Disable interrupts (suspended)
 */

inline void ManagedCircularBuffer::suspend() {
  Nvic::disableAllInterrupts();
  _suspended=true;
}


/*
 * Check if suspended and can be released
 */

inline void ManagedCircularBuffer::checkSuspended() volatile {

  if(_suspended && availableToWrite()>0) {
    _suspended=false;   // must come first
    Nvic::enableAllInterrupts();
  }
}


/*
 * Read a block of bytes
 */

inline void ManagedCircularBuffer::managedRead(uint8_t *output,uint32_t size) volatile {
  circular_buffer<uint8_t>::read(output,size);
  checkSuspended();
}


/*
 * Read a block of bytes
 */

inline uint8_t ManagedCircularBuffer::managedRead() volatile {
  uint8_t b;

  b=circular_buffer<uint8_t>::read();
  checkSuspended();
  return b;
}
