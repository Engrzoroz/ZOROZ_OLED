#include <ZorozOLED.h>

ZorozOLED oled(ZOROZ_OLED_1_3);

void setup() {
  if (!oled.begin()) {
    while (true) {
      delay(1000);
    }
  }

  oled.splash("ENGR ZOROZ", 1200);
  oled.clear(true);
  oled.typewriter("Welcome to ZOROZ OLED", 0, 8, 1, 55, true);
  delay(500);
  oled.clear(true);
  oled.spinner(1400);
  oled.clear();
  oled.centeredText("LOADING", 24, 1);
  oled.animateProgress(100, 15, 4, 44, 120, 14);
  delay(500);
  oled.scrollText("SH1106 AND SSD1306 MADE EASY", 24, 1, 18, 1);
}

void loop() {
}
