#include <ZorozOLED.h>

ZorozOLED oled(ZOROZ_OLED_1_3);

void setup() {
  if (!oled.begin()) {
    while (true) {
      delay(1000);
    }
  }

  oled.clear();
  oled.printLine("ZOROZ OLED", 0, ZOROZ_ALIGN_CENTER, 1);
  oled.printLine("Easy Library", 2, ZOROZ_ALIGN_LEFT, 1);
  oled.printLine("SH1106 + SSD1306", 4, ZOROZ_ALIGN_CENTER, 1);
  oled.printLine("By Engr Zoroz", 6, ZOROZ_ALIGN_RIGHT, 1);
  oled.show();
}

void loop() {
}
