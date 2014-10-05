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
    CommandReader& _reader;
    ManagedCircularBuffer& _commandBuffer;
    Panel& _panel;
    TPinManager _tpinManager;
    StatusIndicators _indicators;

  protected:
    void processNextCommand();

  public:
    CommandExecutor(CommandReader& reader,Panel& panel,StatusIndicators& indicators);
    void run();
};
