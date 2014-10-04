/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once

#include "config/stm32plus.h"
#include "config/i2c.h"
#include "config/spi.h"
#include "config/smartptr.h"
#include "memory/circular_buffer.h"
#include "config/display/tft.h"

using namespace stm32plus;
using namespace stm32plus::display;

#include "CommandReaderInputStream.h"
#include "FontManager.h"
#include "Panel.h"
#include "Flash.h"
#include "FlashInputStream.h"
#include "TPin.h"
#include "TPinManager.h"

#include "../common/CommandId.h"

#include "commands/SetBacklight.h"
#include "commands/SetWindow.h"
#include "commands/Colour.h"
#include "commands/RectangleOps.h"
#include "commands/GradientRect.h"
#include "commands/EllipseOps.h"
#include "commands/Polyline.h"
#include "commands/Line.h"
#include "commands/Plot.h"
#include "commands/BeginWriting.h"
#include "commands/WriteData.h"
#include "commands/Sleep.h"
#include "commands/Wake.h"
#include "commands/Gamma.h"
#include "commands/FlashEraseDevice.h"
#include "commands/FlashEraseSector.h"
#include "commands/FlashProgram.h"
#include "commands/JpegWriter.h"
#include "commands/BitmapWriter.h"
#include "commands/FlashBitmapWriter.h"
#include "commands/SetFont.h"
#include "commands/TextOps.h"
#include "commands/Clear.h"
#include "commands/LowPower.h"
#include "commands/tpin/TpinGpioMode.h"
#include "commands/tpin/TpinGpioControl.h"
#include "commands/tpin/TpinTimerFrequency.h"
#include "commands/tpin/TpinTimerInitCompare.h"
#include "commands/tpin/TpinTimerSetCompare.h"
#include "commands/tpin/TpinTimerControl.h"
#include "commands/tpin/TpinGpioControl.h"

#include "StatusIndicators.h"
#include "CommandReader.h"
#include "CommandExecutor.h"
