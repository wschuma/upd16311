#include "Kenwood_DVF3070.h"

// Change pins as desired
#define VFD_MOSI	A0
#define VFD_MISO	VFD_MOSI
#define VFD_CLK		A1
#define VFD_STB		A2

Kenwood_DVF3070 vfd(VFD_MOSI, VFD_MISO, VFD_CLK, VFD_STB);

void displOffTest()
{
  vfd.println("DISP OFF");
  vfd.updateScreen();
  delay(2000);
  vfd.setDisplayOff();
  delay(2000);
  vfd.setDisplayOn();
  vfd.println("DISP ON");
  vfd.updateScreen();
  delay(2000);
}

void dimmingTest()
{
  char buf[10] = "DIMMING  ";
  for (uint8_t level = 0; level < 8; level++)
  {
    vfd.setDimming(level);
    buf[8] = '0' + level;
    vfd.println(buf);
    vfd.updateScreen();
    delay(2000);
  }
  vfd.setDimming(0);
}

void millisToClock(char *buf)
{
  unsigned long seconds = millis() / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  seconds %= 60;
  minutes %= 60;
  hours %= 24;
  buf += 4;
  if (hours / 10 == 0) {
    *buf++ = ' ';
  } else {
    *buf++ = '0' + hours / 10;
  }
  *buf++ = '0' + hours % 10;
  *buf++ = '0' + minutes / 10;
  *buf++ = '0' + minutes % 10;
  *buf++ = '0' + seconds / 10;
  *buf++ = '0' + seconds % 10;
}

void clockTest()
{
  unsigned long start = millis();
  unsigned long ende = start + 12000;
  unsigned long upd = 0;
  char buf[11] = "    000000";

  vfd.clearScreen();
  vfd.setIndicator(DVF3070_Indicator::SEC, true);
  vfd.setIndicator(DVF3070_Indicator::MIN, true);
  vfd.setIndicator(DVF3070_Indicator::HOUR, true);
  vfd.setIndicator(DVF3070_Indicator::COL1, true);
  vfd.setIndicator(DVF3070_Indicator::COL2, true);
  
  while (upd < ende) {
    if (millis() > upd)
    {
      millisToClock(buf);
      vfd.println(buf);
      vfd.updateScreen();
      upd = millis() + 1000;
    }
  };
}

void setup() {
  Serial.begin(115200);
  vfd.begin();

  // show normal text
  vfd.println("Hello World", false);
  vfd.updateScreen();
  delay(2000);

  // show text in upper case
  vfd.println("Hello World");
  vfd.updateScreen();
  delay(2000);

  dimmingTest();

  clockTest();
  
  displOffTest();

  // show scrolling text
  vfd.clearScreen();
  vfd.setIndicator(DVF3070_Indicator::TITLE, true);
  vfd.println("This is a long text");
}

void loop() {
  vfd.loop();
  static uint32_t upd = 0;
  if (millis() > upd)
  {
    unsigned long keys = vfd.getKey();
    Serial.println(keys, HEX);
    vfd.setIndicator(DVF3070_Indicator::PLAY, (keys & DVF3070_KEY_PLAY));
    vfd.setIndicator(DVF3070_Indicator::PAUSE, !(keys & DVF3070_KEY_PLAY));
    vfd.setIndicator(DVF3070_Indicator::S1, (keys & DVF3070_KEY_REW));
    vfd.setIndicator(DVF3070_Indicator::S2, (keys & DVF3070_KEY_REW));
    vfd.setIndicator(DVF3070_Indicator::S3, (keys & DVF3070_KEY_FFW));
    vfd.setIndicator(DVF3070_Indicator::S4, (keys & DVF3070_KEY_FFW));
    upd = millis() + 500;
  }
}
