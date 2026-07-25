#include <ZorozOLED.h>

ZorozOLED oled(ZOROZ_OLED_0_96);

void setup() {
  Serial.begin(115200);

  if (!oled.begin()) {
    Serial.println("ZOROZ OLED: SSD1306 display not found");
    while (true) {
      delay(1000);
    }
  }

  oled.showText("HELLO!", 2);
}

void loop() {
}
