/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once

#include "config/stm32plus.h"
#include "config/i2c.h"
#include "config/smartptr.h"
#include "config/display/tft.h"

using namespace stm32plus;
using namespace stm32plus::display;

#include "commands/CommandBase.h"
#include "commands/SetBacklight.h"

#include "StatusIndicators.h"
#include "Panel.h"
#include "CommandReader.h"
#include "CommandExecutor.h"
