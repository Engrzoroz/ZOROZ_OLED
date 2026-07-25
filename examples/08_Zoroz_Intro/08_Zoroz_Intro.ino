#include <ZorozOLED.h>

ZorozOLED oled(ZOROZ_OLED_1_3);

void setup() {
  if (!oled.begin()) {
    while (true) {
      delay(1000);
    }
  }

  oled.splash("ENGR ZOROZ", 1200);

  oled.clear();
  oled.drawIcon(ZOROZ_ICON_BELL, 56, 8, 1);
  oled.centeredText("NOTIFICATIONS", 30, 1);
  oled.show();
  delay(800);

  oled.clear();
  oled.drawIcon(ZOROZ_ICON_HEART, 56, 8, 1);
  oled.centeredText("LIKE", 30, 2);
  oled.show();
  delay(800);

  oled.clear();
  oled.drawIcon(ZOROZ_ICON_PLAY, 56, 8, 1);
  oled.centeredText("SUBSCRIBE", 30, 1);
  oled.show();
  delay(800);

  oled.clear();
  oled.centeredText("ENGR ZOROZ", 14, 2);
  oled.progressBar(100, 14, 44, 100, 10, false);
  oled.show();
}

void loop() {
}
