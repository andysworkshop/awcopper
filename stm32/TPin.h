/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


/*
 * Timer pin manager
 */

template<class TTimer,class TGpio>
class TPin {

  protected:

    /*
     * Timer instance. This will be nullptr if the pin is configured for GPIO
     */

    scoped_ptr<TTimer> _ttimer;

  public:
    void enableGpio(GPIOSpeed_TypeDef speed,Gpio::GpioOutputType outputType);
    void enableTimer(uint32_t period,uint16_t prescaler,uint16_t clockDivision,uint16_t counterMode);
};


/*
 * Enable this pin for GPIO
 */

template<class TTimer,class TGpio>
inline void TPin<TTimer,TGpio>::enableGpio(GPIOSpeed_TypeDef speed,Gpio::GpioOutputType outputType) {

  // not using the timer

  _ttimer.reset(nullptr);

  // pin initialisation

  GpioPinInitialiser::initialise(
      reinterpret_cast<GPIO_TypeDef *>(TGpio::Port),
      TGpio::Pin,
      Gpio::OUTPUT,
      speed,
      Gpio::PUPD_NONE,
      outputType,
      0);
}


/*
 * Enable this pin for timer use
 */

template<class TTimer,class TGpio>
inline void TPin<TTimer,TGpio>::enableTimer(uint32_t period,uint16_t prescaler,uint16_t clockDivision,uint16_t counterMode) {

  // create a new timer peripheral

  _ttimer.reset(new TTimer);

  // set up the timebase

  _ttimer->initialiseTimeBase(period,prescaler,clockDivision,counterMode);
}
