#include <SPI.h>
#include <ShiftBrite.h>

#define NumLEDs 8
bool ReverseUpdateAll = true;

//SPI: 10 (SS), 11 (MOSI), 12 (MISO), 13 (SCK).
//ShiftBrite(uint8_t data, uint8_t latch, uint8_t enable, uint8_t clock);
ShiftBrite Lights(11,9,10,13);
int Leds[NumLEDs][3];

unsigned long lastReceivedCmdTime;
#define CmdTimeOut 30000

void setup() {
  Serial.begin(38400);
}

void loop() {
  slowLoop();
  
  //Handle Serial communication.
  //Send 0x55 0xAA {NumLEDs * 2 * 3 bytes, [R][R][G][G][B][B], [R][R][G][G][B][B], [R][...}      Update all leds.
  //Send 0x55 0xBB [Led Number] [R][R][G][G][B][B], Total 9 Bytes.                               Update a single led.
  while(Serial.available() >= 8) {
    lastReceivedCmdTime = millis();
    if (nextByte() == 0x55) {
      byte cmd = nextByte();
      if (cmd == 0xAA) {
        updateLedsFromSerial ();
      } else if (cmd == 0xBB) {
        byte iCurrLed = nextByte();
        if(iCurrLed >= NumLEDs) {
          return;
        }
        Leds[iCurrLed][0] = nextInt();
        Leds[iCurrLed][1] = nextInt();
        Leds[iCurrLed][2] = nextInt();
      }
    }
  }
  
  //Timeout overflow check.
  if (lastReceivedCmdTime > millis()) {
    lastReceivedCmdTime = 0;
  }
  if (millis() > lastReceivedCmdTime + CmdTimeOut) {
    //Timed out
    onSerialIdle();
  } else {
    updateLedColors();
  }
}

//Same as loop but executed less often.
void slowLoop() {
  static byte nextUpdate;
  if (nextUpdate < 128) {
    nextUpdate++;
    return;
  }
  nextUpdate = 0;
  
  updateCurrentRegister();
}

//Update colors for all the leds.
void updateLedColors() {
  for(int i = NumLEDs - 1; i >= 0; i--) {
    Lights.SendColor(Leds[i][0], Leds[i][1], Leds[i][2]);
  }
  Lights.LatchData();
}

//Update colors for all the leds.
void setLedColors(int r, int g, int b) {
  for(int i = NumLEDs - 1; i >= 0; i--) {
    Lights.SendColor(r, g, b);
  }
  Lights.LatchData();
}


//Update current register for all leds.
void updateCurrentRegister () {
  for(int i = 0; i < NumLEDs; i++) {
    Lights.SendCurrent(120, 100, 100);
  }
  Lights.LatchData();
}

//Read leds data from Serial port.
//Read 2 * NumLEDs * 3 bytes from the serial port.
void updateLedsFromSerial() {
  if (ReverseUpdateAll) {
    for (int i = NumLEDs - 1; i >= 0; i--) {
      for (byte u = 0; u < 3; u++) {
        Leds[i][u] = nextInt();
      }
    }
  } else {
    for (int i = 0; i < NumLEDs; i++) {
      for (byte u = 0; u < 3; u++) {
        Leds[i][u] = nextInt();
      }
    }
  }
}

//Executed when serial is idle.
void onSerialIdle() {
  //Turn off the leds when they are not in use.
  setLedColors(0, 0, 0);
}

byte nextByte() {
  while(true) {
    if(Serial.available() > 0) {
      byte b = Serial.read();
      return b;
    }
  }
}

int nextInt() {
  byte high = nextByte();
  byte low = nextByte();
  return (high << 8) | low;
}

