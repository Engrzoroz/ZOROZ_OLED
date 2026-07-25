#include <ZorozOLED.h>

ZorozOLED oled(ZOROZ_OLED_1_3);

void setup() {
  if (!oled.begin()) {
    while (true) {
      delay(1000);
    }
  }

  Adafruit_GFX *canvas = oled.gfx();

  oled.clear();
  canvas->drawRoundRect(0, 0, 128, 64, 8, ZOROZ_WHITE);
  canvas->drawCircle(64, 32, 18, ZOROZ_WHITE);
  canvas->drawLine(0, 0, 127, 63, ZOROZ_WHITE);
  canvas->drawLine(127, 0, 0, 63, ZOROZ_WHITE);
  oled.show();
}

void loop() {
}
