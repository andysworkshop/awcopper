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

    /*
     * Peripheral definitions
     */

    enum {
      I2C_ADDRESS = 0x38        // peripheral slave address
    };


    /*
     * Declare a type for the I2C peripheral
     */

    typedef I2C2_Remap1<I2C2InterruptFeature> MyI2C;

    ManagedCircularBuffer& _commandBuffer;
    StatusIndicators _indicators;
    scoped_ptr<MyI2C> _i2c;
    bool _addressReceived;

  protected:
    void onInterrupt(I2CEventType eventType);

  public:
    CommandReader(ManagedCircularBuffer& commandBuffer,StatusIndicators& indicators);
    void start();

    ManagedCircularBuffer& getCommandBuffer() const;
};


/*
 * Get the command buffer reference
 */

inline ManagedCircularBuffer& CommandReader::getCommandBuffer() const {
  return _commandBuffer;
}

