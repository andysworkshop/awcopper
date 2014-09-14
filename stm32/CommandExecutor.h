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

class CommandExecutor {

  protected:
    CircularBufferInputOutputStream& _commandBuffer;
    Panel& _panel;
    StatusIndicators _indicators;

  public:
    CommandExecutor(CircularBufferInputOutputStream& commandBuffer,Panel& panel,StatusIndicators& indicators);
    void run();
};
