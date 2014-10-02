/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace awc {

  /*
   * Simple struct wrapper for a byte buffer and the number of bytes it contains. This allows
   * streaming variable size buffers into the coprocessor class. No ownership of the pointer
   * is assumed here
   */

  struct Bytes {
    const void *_ptr;
    uint32_t _count;

    Bytes(const void *ptr,uint32_t count)
      : _ptr(ptr),
        _count(count) 
    }
  };
}
