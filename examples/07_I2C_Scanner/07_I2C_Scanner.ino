#include <Arduino.h>
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(1000);
  Serial.println("ZOROZ OLED I2C Scanner");
}

void loop() {
  uint8_t devices = 0;

  for (uint8_t address = 1; address < 127; ++address) {
    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Found device at 0x");
      if (address < 16) {
        Serial.print('0');
      }
      Serial.println(address, HEX);
      ++devices;
    }
  }

  if (devices == 0) {
    Serial.println("No I2C device found");
  } else {
    Serial.println("Scan complete");
  }

  delay(5000);
}
