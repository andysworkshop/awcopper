/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


/*
 * The command reader receives data from the I2C peripheral and writes it into the
 * circular buffer.
 */

class CommandReader {

  protected:

    enum {
      I2C_ADDRESS = 0x43        // peripheral slave address
    };

    /*
     * Declare a type for the I2C peripheral
     */

    typedef I2C2_Remap1<I2C2InterruptFeature> MyI2C;

    CircularBufferInputOutputStream& _commandBuffer;
    StatusIndicators _indicators;
    scoped_ptr<MyI2C> _i2c;

  protected:
    void onInterrupt(I2CEventType eventType);

  public:
    CommandReader(CircularBufferInputOutputStream& commandBuffer,StatusIndicators& indicators);
    void start();
};
