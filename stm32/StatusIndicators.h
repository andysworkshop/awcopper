/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


/*
 * The busy indicator LED on PA9 lights when commands are being executed. The full indicator
 * on PA10 lights up if the command buffer fills up (overflow condition)
 */

class StatusIndicators {

  protected:

    /*
     * Pins on port A
     */

    enum {
      BUSY_PIN = 9,
      FULL_PIN = 10
    };

    GpioPinRef _full;
    GpioPinRef _busy;

  public:
    StatusIndicators();

    void setFull(bool full) const;
    void setBusy(bool busy) const;
};



inline StatusIndicators::StatusIndicators() {

  // initialise the pins

  GpioA<DefaultDigitalOutputFeature<BUSY_PIN,FULL_PIN>> pa;

  // stash simple references to the pins

  _busy=pa[BUSY_PIN];
  _full=pa[FULL_PIN];
}


/*
 * Set the state of the full indicator
 */

inline void StatusIndicators::setFull(bool full) const {
  _full.setState(full);
}


/*
 * Set the state of the busy indicator
 */

inline void StatusIndicators::setBusy(bool busy) const {
  _busy.setState(busy);
}
