/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#include "Application.h"


/*
 * Main entry point
 */

int main() {

  // initialise the SysTick timer

  MillisecondTimer::initialise();

  // initialise the NVIC

  Nvic::initialise();

  // declare and initialise the LCD panel

  Panel panel;

  // declare the 1Kb circular buffer that will be used to buffer commands
  // received from the arduino before they're executed by the STM32

  ManagedCircularBuffer commandBuffer(1024);

  // declare the status indicators

  StatusIndicators indicators;

  // declare the I2C reader that receives data via the interrupts

  CommandReader reader(commandBuffer,indicators);

  // declare the command reader

  CommandExecutor executor(reader,panel,indicators);

  // start the reader and run the executor

  reader.start();
  executor.run();

  // not reached
  return 0;
}
