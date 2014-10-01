/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace awc {

  /*
   * Helper class to manage writing a stream out to the Wire library that's potentially
   * larger than it's internal buffer size
   */

  class WireStream {
    
    protected:
      uint8_t _bufindex;

    protected:
      void flush();

    public:
      WireStream(uint8_t command);
      ~WireStream();

      void write(uint8_t b);
  };
}