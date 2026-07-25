#include <ZorozOLED.h>

ZorozOLED oled(ZOROZ_OLED_0_96);

void setup() {
  if (!oled.begin()) {
    while (true) {
      delay(1000);
    }
  }

  oled.clear();
  oled.drawIcon(ZOROZ_ICON_HEART, 4, 2);
  oled.drawIcon(ZOROZ_ICON_BELL, 28, 2);
  oled.drawIcon(ZOROZ_ICON_PLAY, 52, 2);
  oled.wifi(75, 78, 2);
  oled.battery(80, 100, 4, 27, 10);
  oled.centeredText("SYSTEM READY", 24, 1);
  oled.progressBar(80, 4, 44, 120, 14, true);
  oled.show();
}

void loop() {
}
