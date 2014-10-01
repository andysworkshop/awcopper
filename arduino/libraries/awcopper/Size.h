/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace awc {

  /*
   * Size structure
   */

  struct Size {

    /*
     * The width and height
     */

    int16_t Width,Height;


    /*
     * Default constructor
     */

    Size()
      : Width(), Height() {
    }


    /*
     * Constructor with parameters
     */

    Size(int16_t width,int16_t height) :
      Width(width),
      Height(height) {
    }


    /*
     * Copy constructor
     */

    Size(const Size& src) {
      Width=src.Width;
      Height=src.Height;
    }


    /*
     * Get the area covered by this size
     */

    uint32_t getArea() const {
      return static_cast<uint32_t>(Width)*static_cast<uint32_t>(Height);
    }
  };
}
