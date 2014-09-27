/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


/*
 * This class looks after the "T" pins. T1 and T2 are connected to PA8 and PA3 respectively.
 * PA8 is the output pin for TIM1 channel 1 and PA3 is for TIM2 channel 4. TIM1 is an 'advanced'
 * control timer with a 16-bit reload counter. TIM2 is a 'general purpose' timer with a 32-bit
 * reload counter.
 *
 * We allow the pins to configured as plain GPIO output pins or as PWM outputs.
 */

class TPinManager {

  public:

    /*
     * The pin selector. The numbers matter because the Arduino will send them and we
     * just cast to this enum.
     */

    enum class TPinNumber {
      T1 = 1,
      T2 = 2
    };

  protected:

    /*
     * Timer configuration for T1
     */

    typedef Timer1<
        Timer1InternalClockFeature,     // the timer clock source is APB
        TimerChannel1Feature<>,         // we're going to use channel 1
        Timer2GpioFeature<              // we want to output something to GPIO
          TIMER_REMAP_NONE,             // the GPIO output will not be remapped
          TIM1_CH1_OUT                  // we will output channel 1 to GPIO
        >
      > T1_TimerType;

    /*
     * Timer configuration for T2
     */

    typedef Timer2<
        Timer2InternalClockFeature,     // the timer clock source is APB
        TimerChannel4Feature<>,         // we're going to use channel 4
        Timer2GpioFeature<              // we want to output something to GPIO
          TIMER_REMAP_NONE,             // the GPIO output will not be remapped
          TIM2_CH4_OUT                  // we will output channel 4 to GPIO
        >
      > T2_TimerType;

    /*
     * Instantiations of the TPin template for the two pins
     */

    TPin<T1_TimerType,gpio::PA8> _t1;
    TPin<T2_TimerType,gpio::PA3> _t2;

  public:
    void enableGpio(TPinNumber pinNumber,GPIOSpeed_TypeDef speed,Gpio::GpioOutputType outputType);
    void enableTimer(TPinNumber pinNumber,uint32_t period,uint16_t prescaler,uint16_t clockDivision,uint16_t counterMode);
};


/*
 * Enable a pin for GPIO
 */

inline void TPinManager::enableGpio(TPinNumber pinNumber,GPIOSpeed_TypeDef speed,Gpio::GpioOutputType outputType) {

  if(pinNumber==TPinNumber::T1)
    _t1.enableGpio(speed,outputType);
  else
    _t2.enableGpio(speed,outputType);
}


/*
 * Enable a pin for timing
 */

inline void TPinManager::enableTimer(TPinNumber pinNumber,uint32_t period,uint16_t prescaler,uint16_t clockDivision,uint16_t counterMode) {

  if(pinNumber==TPinNumber::T1)
    _t1.enableTimer(period,prescaler,clockDivision,counterMode);
  else
    _t2.enableTimer(period,prescaler,clockDivision,counterMode);
}
