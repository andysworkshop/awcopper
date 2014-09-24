/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Rectangle operations (outlined, filled, cleared)
   *
   * Parameters:
   *   0,1  : top-left X
   *   2,3  : top-left Y
   *   4..n : ASCII string (null terminated)
   */

  struct TextOps {

    enum {
      PARAMETER_COUNT = 4,

      FILLED,
      OUTLINED
    };

    static void execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer,int operType);
  };


  /*
   * Execute the command
   */

  inline void TextOps::execute(Panel& panel,circular_buffer<uint8_t>& commandBuffer,int operType) {

    Panel::LcdPanel& gl(panel.getGraphicsLibrary());
    int16_t parameters[2];

    // wait for, and then read the parameters

    while(commandBuffer.availableToRead()<PARAMETER_COUNT);
    commandBuffer.read(reinterpret_cast<uint8_t *>(parameters),PARAMETER_COUNT);

    Point pos(parameters[0],parameters[1]);
    const FontChar *fc;
    int16_t width;
    Size s;
    uint8_t c;

    // get the selected font

    const Font &font(*gl.getStreamSelectedFont());

    // set initial size with constant height

    s.Height=font.getHeight();
    s.Width=0;

    // print each character in turn

    for(;;) {

      // wait for data

      while(commandBuffer.availableToRead()==0);

      // break out when zero is read

      if((c=commandBuffer.read())==0)
        break;

      font.getCharacter(c,fc);

      if(operType==FILLED)
        gl.writeCharacterFill(pos,font,*fc);
      else
        gl.writeCharacterNoFill(pos,font,*fc);

      width=fc->PixelWidth+font.getCharacterSpacing();
      pos.X+=width;
      s.Width+=width;
    }
  }
}

