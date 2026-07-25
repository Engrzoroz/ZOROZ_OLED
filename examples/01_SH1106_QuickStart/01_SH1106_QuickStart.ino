#include <ZorozOLED.h>

ZorozOLED oled(ZOROZ_OLED_1_3);

void setup() {
  Serial.begin(115200);

  if (!oled.begin()) {
    Serial.println("ZOROZ OLED: SH1106 display not found");
    while (true) {
      delay(1000);
    }
  }

  oled.splash("ENGR ZOROZ");
  oled.showMessage("ZOROZ OLED", "1.3 inch SH1106 is ready!", ZOROZ_ICON_CHECK);
}

void loop() {
}
