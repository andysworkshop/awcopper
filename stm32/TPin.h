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
    void setupGpio(GPIOSpeed_TypeDef speed,Gpio::GpioOutputType outputType);
    void setupTimer(uint32_t period,uint16_t prescaler,uint16_t clockDivision,uint16_t counterMode);

    void controlTimer(bool enable) const;
    void controlGpio(bool set) const;

    void initCompare(uint32_t compareValue,uint16_t ocMode,uint16_t polarity,uint16_t preload) const;
    void setCompare(uint32_t compareValue) const;
};


/*
 * Enable this pin for GPIO
 */

template<class TTimer,class TGpio>
inline void TPin<TTimer,TGpio>::setupGpio(GPIOSpeed_TypeDef speed,Gpio::GpioOutputType outputType) {

  // not using the timer

  _ttimer.reset(nullptr);

  // pin initialisation

  GpioPinInitialiser::initialise(
      reinterpret_cast<GPIO_TypeDef *>(TGpio::Port),
      TGpio::Pin,
      Gpio::OUTPUT,
      speed,
      Gpio::PUPD_NONE,
      outputType);
}


/*
 * Enable this pin for timer use
 */

template<class TTimer,class TGpio>
inline void TPin<TTimer,TGpio>::setupTimer(uint32_t period,uint16_t prescaler,uint16_t clockDivision,uint16_t counterMode) {

  // create a new timer peripheral

  _ttimer.reset(new TTimer);

  // set up the timebase

  _ttimer->initialiseTimeBase(period,prescaler,clockDivision,counterMode);
}


/*
 * Setup the channel OC configuration
 */

template<class TTimer,class TGpio>
inline void TPin<TTimer,TGpio>::initCompare(uint32_t compareValue,uint16_t ocMode,uint16_t polarity,uint16_t preload) const {
  _ttimer->initCompare(compareValue,ocMode,polarity,preload);
}


/*
 * Set the compare value for the channel
 */

template<class TTimer,class TGpio>
inline void TPin<TTimer,TGpio>::setCompare(uint32_t compareValue) const {
  _ttimer->setCompare(compareValue);
}


/*
 * Enable/disable the timer
 */

template<class TTimer,class TGpio>
inline void TPin<TTimer,TGpio>::controlTimer(bool enable) const {

  if(enable)
    _ttimer->enablePeripheral();
  else
    _ttimer->disablePeripheral();
}


/*
 * Set the GPIO pin level
 */

template<class TTimer,class TGpio>
inline void TPin<TTimer,TGpio>::controlGpio(bool set) const {

  // go direct to the registers

  if(set)
    reinterpret_cast<GPIO_TypeDef *>(TGpio::Port)->BSRR=TGpio::Pin;
  else
    reinterpret_cast<GPIO_TypeDef *>(TGpio::Port)->BRR=TGpio::Pin;
}
