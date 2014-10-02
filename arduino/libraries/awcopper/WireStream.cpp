/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include "awcopper.h"
#include "WireStream.h"


namespace awc {


  /*
   * Constructors
   */

  WireStream::WireStream() {
    _bufindex=0;
  }

  WireStream::WireStream(uint8_t command) {
    *CoProcessor::buffer=command;
    _bufindex=1;
  }


  /*
   * Destructor
   */

  WireStream::~WireStream() {
    flush();
  }


  /*
   * If the buffer has bytes, flush it
   */

  void WireStream::flush() {

    if(_bufindex) {

      Wire.beginTransmission(CoProcessor::SLAVE_ADDRESS);
      Wire.write(CoProcessor::buffer,_bufindex);
      Wire.endTransmission();

      _bufindex=0;
    }
  }


  /*
   * Write a byte
   */

  void WireStream::write(uint8_t b) {

    if(_bufindex==BUFFER_LENGTH)
      flush();

    CoProcessor::buffer[_bufindex++]=b;
  }


  /*
   * Write a buffer
   */

  void WireStream::write(const void *buffer,uint16_t count) {

    const uint8_t *ptr=reinterpret_cast<const uint8_t *>(buffer);

    while(count--)
      write(*ptr++);
  }
}
