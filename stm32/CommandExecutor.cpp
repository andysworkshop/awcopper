/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#include "Application.h"


/*
 * Constructor
 */

CommandExecutor::CommandExecutor(CommandReader& reader,Panel& panel,StatusIndicators& indicators)
  : _reader(reader),
    _commandBuffer(reader.getCommandBuffer()),
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
      _reader.checkPending();
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

    case CommandId::FONT:
      SetFont::execute(_panel,_commandBuffer);
      break;

    case CommandId::WRITE_TEXT:
      TextOps::execute(_panel,_commandBuffer,TextOps::OUTLINED);
      break;

    case CommandId::WRITE_FILLED_TEXT:
      TextOps::execute(_panel,_commandBuffer,TextOps::FILLED);
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

    case CommandId::WRITE_BITMAP:
      BitmapWriter::execute(_panel,_commandBuffer,BitmapWriter::UNCOMPRESSED);
      break;

    case CommandId::WRITE_LZG_BITMAP:
      BitmapWriter::execute(_panel,_commandBuffer,BitmapWriter::COMPRESSED);
      break;

    case CommandId::WRITE_FLASH_BITMAP:
      FlashBitmapWriter::execute(_panel,_commandBuffer,FlashBitmapWriter::Storage::UNCOMPRESSED);
      break;

    case CommandId::WRITE_FLASH_LZG_BITMAP:
      FlashBitmapWriter::execute(_panel,_commandBuffer,FlashBitmapWriter::Storage::COMPRESSED);
      break;

    case CommandId::WRITE_FLASH_JPEG:
      FlashBitmapWriter::execute(_panel,_commandBuffer,FlashBitmapWriter::Storage::JPEG);
      break;

    case CommandId::TPIN_TIMER_SET_FREQUENCY:
      TpinTimerFrequency::execute(_tpinManager,_commandBuffer);
      break;

    case CommandId::TPIN_TIMER_INIT_COMPARE:
      TpinTimerInitCompare::execute(_tpinManager,_commandBuffer);
      break;

    case CommandId::TPIN_TIMER_SET_COMPARE:
      TpinTimerSetCompare::execute(_tpinManager,_commandBuffer);
      break;

    case CommandId::TPIN_TIMER_CONTROL:
      TpinTimerControl::execute(_tpinManager,_commandBuffer);
      break;

    case CommandId::TPIN_GPIO_CONFIGURE:
      TpinGpioMode::execute(_tpinManager,_commandBuffer);
      break;

    case CommandId::TPIN_GPIO_CONTROL:
      TpinGpioControl::execute(_tpinManager,_commandBuffer);
      break;

    default:
      // invalid command. not good.
      break;
  }
}
