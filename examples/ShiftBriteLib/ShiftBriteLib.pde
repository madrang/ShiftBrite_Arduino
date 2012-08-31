#include <ShiftBrite.h>

#define NumLEDs 8

ShiftBrite Lights(11,9,10,13);

void setup() {
  Serial.begin(38400);
}

void loop() {
  
  //Update current register for all leds.
  for(int i = 0; i < NumLEDs; i++) {
    Lights.SendCurrent(120, 100, 100);
  }
  Lights.LatchData();
  
  //Send the Red color to all the leds.
  for(int i = 0; i < NumLEDs; i++) {
    Lights.SendColor(1023, 0, 0);
  }
  Lights.LatchData();
  
}
