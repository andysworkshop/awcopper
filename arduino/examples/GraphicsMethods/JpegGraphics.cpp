/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */
 
void _asmStub() {

  __asm volatile(

    ".global JpegData\n\t"
    ".global JpegDataSize\n\t"

    "JpegData:\n\t"
    ".incbin \"libraries/awcopper/examples/GraphicsMethods/arduino.jpg\"\n\t"
    "JpegDataSize=.-JpegData\n\t"
  );
}

