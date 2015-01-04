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
 * Constructor
 */

inline CommandReader::CommandReader(ManagedCircularBuffer& commandBuffer,StatusIndicators& indicators)
  : _commandBuffer(commandBuffer),
    _indicators(indicators),
    _addressReceived(false) {

  MyI2C::Parameters params;

  // initialise the peripheral

  params.i2c_timing=0xB0420F13;
  params.i2c_ownAddress=I2C_ADDRESS << 1;
  params.i2c_analogFilter=false;

  _i2c.reset(new MyI2C(params));

  // subscribe to interrupts

  _i2c->I2CInterruptEventSender.insertSubscriber(
      I2CInterruptEventSourceSlot::bind(this,&CommandReader::onInterrupt));
}


/*
 * Start the reader
 */

inline void CommandReader::start() {

  // enable the interrupts and the peripheral

  _i2c->clearPendingInterruptsFlag(I2C_IT_RXI | I2C_IT_TXI | I2C_IT_ERRI | I2C_IT_TCI | I2C_IT_STOPI | I2C_IT_NACKI | I2C_IT_ADDRI);
  _i2c->enableInterrupts(I2C_IT_RXI | I2C_IT_TXI | I2C_IT_ERRI | I2C_IT_TCI | I2C_IT_STOPI | I2C_IT_NACKI | I2C_IT_ADDRI);
  _i2c->enablePeripheral();
}


/*
 * I2C interrupt received
 */

inline void CommandReader::onInterrupt(I2CEventType eventType) {

  bool full;

  switch(eventType) {

    case I2CEventType::EVENT_ADDRESS_MATCH:
      _addressReceived=true;
      break;

    case I2CEventType::EVENT_RECEIVE:                 // data received

      // got some data

      _addressReceived=false;

      // write the byte

      _commandBuffer.write(I2C_ReceiveData(*_i2c));   // add to the circular buffer

      full=_commandBuffer.availableToWrite()==0;
      _indicators.setFull(full);                      // set/reset the full LED

      // is the buffer full? Suspend incoming if it is.

      if(full)
        _commandBuffer.suspend();

      break;

    case I2CEventType::EVENT_STOP_BIT_RECEIVED:
      if(_addressReceived)                            // no data in frame? must be a reset request
        NVIC_SystemReset();
      else
        _addressReceived=false;
      break;

    default:
      break;
  }
}


/*
 * Get the command buffer reference
 */

inline ManagedCircularBuffer& CommandReader::getCommandBuffer() const {
  return _commandBuffer;
}

