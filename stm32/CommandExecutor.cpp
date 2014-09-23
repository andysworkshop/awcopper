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

  using namespace cmd;

  CommandId id;

  // read the command from the buffer

  id=static_cast<CommandId>(_commandBuffer.read());

  // execute the command

  switch(id) {

    case CommandId::SET_BACKLIGHT:
      SetBacklight::execute(_panel,_commandBuffer);
      break;

    case CommandId::SET_WINDOW:
      SetWindow::execute(_panel,_commandBuffer);
      break;

    case CommandId::FOREGROUND_COLOUR:
      Colour::execute(_panel,_commandBuffer,true);
      break;

    case CommandId::BACKGROUND_COLOUR:
      Colour::execute(_panel,_commandBuffer,false);
      break;

    case CommandId::RECTANGLE:
      RectangleOps::execute(_panel,_commandBuffer,RectangleOps::OUTLINED);
      break;

    case CommandId::FILL_RECTANGLE:
      RectangleOps::execute(_panel,_commandBuffer,RectangleOps::FILLED);
      break;

    case CommandId::CLEAR_RECTANGLE:
      RectangleOps::execute(_panel,_commandBuffer,RectangleOps::CLEARED);
      break;

    case CommandId::POLYLINE:
      Polyline::execute(_panel,_commandBuffer);
      break;

    case CommandId::LINE:
      Line::execute(_panel,_commandBuffer);
      break;

    case CommandId::PLOT:
      Plot::execute(_panel,_commandBuffer);
      break;

    case CommandId::ELLIPSE:
      EllipseOps::execute(_panel,_commandBuffer,EllipseOps::OUTLINED);
      break;

    case CommandId::FILL_ELLIPSE:
      EllipseOps::execute(_panel,_commandBuffer,EllipseOps::FILLED);
      break;

    case CommandId::BEGIN_WRITING:
      BeginWriting::execute(_panel);
      break;

    case CommandId::WRITE_DATA:
      WriteData::execute(_panel,_commandBuffer);
      break;

    case CommandId::GAMMA:
      Gamma::execute(_panel,_commandBuffer);
      break;

    case CommandId::ERASE_FLASH_DEVICE:
      FlashEraseDevice::execute();
      break;

    case CommandId::ERASE_FLASH_SECTOR:
      FlashEraseSector::execute(_commandBuffer);
      break;

    case CommandId::PROGRAM_FLASH:
      FlashProgram::execute(_commandBuffer);
      break;

    case CommandId::WRITE_JPEG:
      JpegWriter::execute(_panel,_commandBuffer);
      break;

    default:
      // invalid command. not good.
      break;
  }
}
