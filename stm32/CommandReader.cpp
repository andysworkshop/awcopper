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
    _indicators(indicators) {

  MyI2C::Parameters params;

  // initialise the peripheral

  params.i2c_timing=0x00731012;
  params.i2c_ownAddress=I2C_ADDRESS;

  _i2c.reset(new MyI2C(params));

  // subscribe to interrupts

  _i2c->I2CInterruptEventSender.insertSubscriber(I2CInterruptEventSourceSlot::bind(this,&CommandReader::onInterrupt));
}


/*
 * Start the reader
 */

void CommandReader::start() {

  // enable the interrupts and the peripheral

  _i2c->clearPendingInterruptsFlag(I2C_IT_RXI);
  _i2c->enableInterrupts(I2C_IT_RXI);
  _i2c->enablePeripheral();
}


/*
 * I2C interrupt received
 */

void CommandReader::onInterrupt(I2CEventType eventType) {

  switch(eventType) {

    case I2CEventType::EVENT_RECEIVE:                   // data received
      _commandBuffer.write(I2C_ReceiveData(*_i2c));     // add to the circular buffer
      _indicators.setFull(_commandBuffer.availableToWrite()==0);     // set/reset the full LED
      break;

    default:
      break;
  }
}
