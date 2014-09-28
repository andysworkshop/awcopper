/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#include "Application.h"


/*
 * Constructor
 */

CommandReader::CommandReader(circular_buffer<uint8_t>& commandBuffer,StatusIndicators& indicators)
  : _commandBuffer(commandBuffer),
    _indicators(indicators),
    _suspended(false) {

  MyI2C::Parameters params;

  // initialise the peripheral

  params.i2c_timing=0xB0420F13;
  params.i2c_ownAddress=I2C_ADDRESS << 1;
  params.i2c_analogFilter=false;

  _i2c.reset(new MyI2C(params));

  // subscribe to interrupts

  _i2c->I2CInterruptEventSender.insertSubscriber(I2CInterruptEventSourceSlot::bind(this,&CommandReader::onInterrupt));
}


/*
 * Start the reader
 */

void CommandReader::start() {

  // enable the interrupts and the peripheral

  _i2c->clearPendingInterruptsFlag(I2C_IT_RXI | I2C_IT_TXI | I2C_IT_ERRI | I2C_IT_TCI | I2C_IT_STOPI | I2C_IT_NACKI | I2C_IT_ADDRI);
  _i2c->enableInterrupts(I2C_IT_RXI | I2C_IT_TXI | I2C_IT_ERRI | I2C_IT_TCI | I2C_IT_STOPI | I2C_IT_NACKI | I2C_IT_ADDRI);
  _i2c->enablePeripheral();
}


/*
 * I2C interrupt received
 */

void CommandReader::onInterrupt(I2CEventType eventType) {

  bool full;

  switch(eventType) {

    case I2CEventType::EVENT_RECEIVE:                   // data received

      // write the byte

      _commandBuffer.write(I2C_ReceiveData(*_i2c));   // add to the circular buffer

      full=_commandBuffer.availableToWrite()==0;
      _indicators.setFull(full);                      // set/reset the full LED

      // is the buffer full?

      if(full) {
        _i2c->disableInterrupts(I2C_IT_RXI);
        _suspended=true;                                // SCL is stretched until we read RXDR
      }
      break;

    default:
      break;
  }
}


/*
 * If we're suspended then data is pending and could be cleared
 */

void CommandReader::checkPending() {

  if(_suspended && _commandBuffer.availableToWrite()>0) {
    _suspended=false;
    _i2c->enableInterrupts(I2C_IT_RXI);
  }
}
