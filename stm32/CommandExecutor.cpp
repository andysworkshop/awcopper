/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#include "Application.h"


/*
 * Constructor
 */

CommandExecutor::CommandExecutor(circular_buffer<uint8_t>& commandBuffer,Panel& panel,StatusIndicators& indicators)
  : _commandBuffer(commandBuffer),
    _panel(panel),
    _indicators(indicators ){
}


/*
 * Run the executor and do not return
 */

void CommandExecutor::run() {

  for(;;) {

    // wait for data to become available

    while(_commandBuffer.availableToRead()==0);

    // keep the busy light on while buffered commands are processed

    _indicators.setBusy(true);

    do {
      processNextCommand();
    } while(_commandBuffer.availableToRead()!=0);

    // buffered commands processed, switch off the indicator

    _indicators.setBusy(false);
  }
}


/*
 * Process the next command in the buffer
 */

void CommandExecutor::processNextCommand() {

  cmd::CommandId id;

  // read the command from the buffer

  id=static_cast<cmd::CommandId>(_commandBuffer.read());

  // execute the command

  switch(id) {

    case cmd::CommandId::SET_BACKLIGHT:
      cmd::SetBacklight::execute(_panel,_commandBuffer);
      break;

    case cmd::CommandId::SET_WINDOW:
      cmd::SetWindow::execute(_panel,_commandBuffer);
      break;

    case cmd::CommandId::FOREGROUND_COLOUR:
      cmd::Colour::execute(_panel,_commandBuffer,true);
      break;

    case cmd::CommandId::BACKGROUND_COLOUR:
      cmd::Colour::execute(_panel,_commandBuffer,false);
      break;

    case cmd::CommandId::RECTANGLE:
      cmd::DrawRectangle::execute(_panel,_commandBuffer);
      break;

    default:
      // invalid command. not good.
      break;
  }
}
