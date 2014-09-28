#include <Wire.h>
#include <awcopper.h>

awc::CoProcessor copro;

void setup() {

  copro.begin();
  
  copro << awc::backlight(100);
}

void loop() {
}

