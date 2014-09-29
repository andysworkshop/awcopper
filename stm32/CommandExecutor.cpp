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

  // execute the command

  switch(_commandBuffer.read()) {

    case cmd::SET_BACKLIGHT:
      cmd::SetBacklight::execute(_panel,_commandBuffer);
      break;

    case cmd::SET_WINDOW:
      cmd::SetWindow::execute(_panel,_commandBuffer);
      break;

    case cmd::FOREGROUND_COLOUR:
      cmd::Colour::execute(_panel,_commandBuffer,true);
      break;

    case cmd::BACKGROUND_COLOUR:
      cmd::Colour::execute(_panel,_commandBuffer,false);
      break;

    case cmd::RECTANGLE:
      cmd::RectangleOps::execute(_panel,_commandBuffer,cmd::RectangleOps::OUTLINED);
      break;

    case cmd::FILL_RECTANGLE:
      cmd::RectangleOps::execute(_panel,_commandBuffer,cmd::RectangleOps::FILLED);
      break;

    case cmd::CLEAR_RECTANGLE:
      cmd::RectangleOps::execute(_panel,_commandBuffer,cmd::RectangleOps::CLEARED);
      break;

    case cmd::POLYLINE:
      cmd::Polyline::execute(_panel,_commandBuffer);
      break;

    case cmd::LINE:
      cmd::Line::execute(_panel,_commandBuffer);
      break;

    case cmd::PLOT:
      cmd::Plot::execute(_panel,_commandBuffer);
      break;

    case cmd::ELLIPSE:
      cmd::EllipseOps::execute(_panel,_commandBuffer,cmd::EllipseOps::OUTLINED);
      break;

    case cmd::FILL_ELLIPSE:
      cmd::EllipseOps::execute(_panel,_commandBuffer,cmd::EllipseOps::FILLED);
      break;

    case cmd::BEGIN_WRITING:
      cmd::BeginWriting::execute(_panel);
      break;

    case cmd::WRITE_DATA:
      cmd::WriteData::execute(_panel,_commandBuffer);
      break;

    case cmd::FONT:
      cmd::SetFont::execute(_panel,_commandBuffer);
      break;

    case cmd::WRITE_TEXT:
      cmd::TextOps::execute(_panel,_commandBuffer,cmd::TextOps::OUTLINED);
      break;

    case cmd::WRITE_FILLED_TEXT:
      cmd::TextOps::execute(_panel,_commandBuffer,cmd::TextOps::FILLED);
      break;

    case cmd::GAMMA:
      cmd::Gamma::execute(_panel,_commandBuffer);
      break;

    case cmd::ERASE_FLASH_DEVICE:
      cmd::FlashEraseDevice::execute();
      break;

    case cmd::ERASE_FLASH_SECTOR:
      cmd::FlashEraseSector::execute(_commandBuffer);
      break;

    case cmd::PROGRAM_FLASH:
      cmd::FlashProgram::execute(_commandBuffer);
      break;

    case cmd::WRITE_JPEG:
      cmd::JpegWriter::execute(_panel,_commandBuffer);
      break;

    case cmd::WRITE_BITMAP:
      cmd::BitmapWriter::execute(_panel,_commandBuffer,cmd::BitmapWriter::UNCOMPRESSED);
      break;

    case cmd::WRITE_LZG_BITMAP:
      cmd::BitmapWriter::execute(_panel,_commandBuffer,cmd::BitmapWriter::COMPRESSED);
      break;

    case cmd::WRITE_FLASH_BITMAP:
      cmd::FlashBitmapWriter::execute(_panel,_commandBuffer,cmd::FlashBitmapWriter::Storage::UNCOMPRESSED);
      break;

    case cmd::WRITE_FLASH_LZG_BITMAP:
      cmd::FlashBitmapWriter::execute(_panel,_commandBuffer,cmd::FlashBitmapWriter::Storage::COMPRESSED);
      break;

    case cmd::WRITE_FLASH_JPEG:
      cmd::FlashBitmapWriter::execute(_panel,_commandBuffer,cmd::FlashBitmapWriter::Storage::JPEG);
      break;

    case cmd::TPIN_TIMER_SET_FREQUENCY:
      cmd::TpinTimerFrequency::execute(_tpinManager,_commandBuffer);
      break;

    case cmd::TPIN_TIMER_INIT_COMPARE:
      cmd::TpinTimerInitCompare::execute(_tpinManager,_commandBuffer);
      break;

    case cmd::TPIN_TIMER_SET_COMPARE:
      cmd::TpinTimerSetCompare::execute(_tpinManager,_commandBuffer);
      break;

    case cmd::TPIN_TIMER_CONTROL:
      cmd::TpinTimerControl::execute(_tpinManager,_commandBuffer);
      break;

    case cmd::TPIN_GPIO_CONFIGURE:
      cmd::TpinGpioMode::execute(_tpinManager,_commandBuffer);
      break;

    case cmd::TPIN_GPIO_CONTROL:
      cmd::TpinGpioControl::execute(_tpinManager,_commandBuffer);
      break;

    default:
      // invalid command. not good.
      break;
  }
}
