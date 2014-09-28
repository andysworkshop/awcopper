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
      I2C_ADDRESS = 0x38        // peripheral slave address
    };

    /*
     * Declare a type for the I2C peripheral
     */

    typedef I2C2_Remap1<I2C2InterruptFeature> MyI2C;

    circular_buffer<uint8_t>& _commandBuffer;
    StatusIndicators _indicators;
    scoped_ptr<MyI2C> _i2c;
    volatile bool _pending;

  protected:
    void onInterrupt(I2CEventType eventType);

  public:
    CommandReader(circular_buffer<uint8_t>& commandBuffer,StatusIndicators& indicators);
    void start();

    void checkPending();
    circular_buffer<uint8_t>& getCommandBuffer() const;
};


/*
 * Get the command buffer reference
 */

inline circular_buffer<uint8_t>& CommandReader::getCommandBuffer() const {
  return _commandBuffer;
}

