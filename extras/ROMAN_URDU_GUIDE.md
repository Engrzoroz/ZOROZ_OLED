# ZOROZ OLED — Roman Urdu Complete Guide

## Library ka purpose

ZOROZ OLED common monochrome I2C OLED displays ko simple functions se control karti hai. Version 1.0.0 mein yeh displays supported hain:

- 1.3 inch 128x64 SH1106
- 0.96 inch 128x64 SSD1306
- SSD1306 128x32

Screen ka physical size controller ki guarantee nahi hota. 1.3 inch module aam tor par SH1106 aur 0.96 inch module aam tor par SSD1306 hota hai, lekin seller ki specification zaroor check karo.

## Required dependency libraries

Arduino IDE ke Library Manager se install karo:

1. Adafruit GFX Library
2. Adafruit SH110X
3. Adafruit SSD1306

ZOROZ OLED low-level communication ke liye in tested drivers ko use karti hai aur user ko short, easy API deti hai.

## ZIP install karna

1. Arduino IDE open karo.
2. `Sketch > Include Library > Add .ZIP Library` open karo.
3. `ZOROZ_OLED_v1.0.0.zip` select karo.
4. Arduino IDE restart karo.
5. `File > Examples > ZOROZ OLED` open karo.

## Basic wiring

### Arduino Uno/Nano

- OLED GND → GND
- OLED VCC → module ke mutabiq 3.3V ya 5V
- OLED SDA → A4
- OLED SCL → A5

### Arduino Mega

- OLED GND → GND
- OLED VCC → module ke mutabiq 3.3V ya 5V
- OLED SDA → pin 20
- OLED SCL → pin 21

### Typical ESP32 DevKit

- OLED GND → GND
- OLED VCC → 3.3V
- OLED SDA → GPIO 21
- OLED SCL → GPIO 22

Exact ESP32 board ke default I2C pins different ho sakte hain.

## 1.3 inch SH1106 ka sab se easy code

```cpp
#include <ZorozOLED.h>

ZorozOLED oled(ZOROZ_OLED_1_3);

void setup() {
  if (!oled.begin()) {
    while (true) {
      delay(1000);
    }
  }

  oled.showText("HELLO", 2);
}

void loop() {
}
```

## 0.96 inch SSD1306 ka sab se easy code

```cpp
#include <ZorozOLED.h>

ZorozOLED oled(ZOROZ_OLED_0_96);

void setup() {
  if (!oled.begin()) {
    while (true) {
      delay(1000);
    }
  }

  oled.showMessage("ZOROZ OLED", "Display ready!", ZOROZ_ICON_CHECK);
}

void loop() {
}
```

## Address change karna

Default address `0x3C` hai. Agar I2C scanner `0x3D` show kare:

```cpp
ZorozOLED oled(ZOROZ_OLED_0_96, 0x3D);
```

SH1106 aur SSD1306 dono modules `0x3C` use kar sakte hain, is liye address se controller type reliably identify nahi hota. Correct model constructor mein select karo.

## Frame draw karne ka correct method

Pehle buffer mein sab draw karo, phir aik martaba `show()` call karo:

```cpp
oled.clear();
oled.centeredText("ENGR ZOROZ", 2, 1);
oled.drawIcon(ZOROZ_ICON_HEART, 56, 20);
oled.progressBar(75, 4, 45, 120, 12, true);
oled.show();
```

Is se unnecessary refresh aur flicker kam hota hai.

## Easy text functions

```cpp
oled.showText("HELLO", 2);
oled.centeredText("CENTER", 20, 1);
oled.printLine("LEFT", 0, ZOROZ_ALIGN_LEFT, 1);
oled.printLine("CENTER", 2, ZOROZ_ALIGN_CENTER, 1);
oled.printLine("RIGHT", 4, ZOROZ_ALIGN_RIGHT, 1);
oled.wrappedText("Long message automatically lines mein divide ho ga", 2, 16, 124);
```

## Normal Arduino print support

```cpp
oled.clear();
oled.setCursor(0, 0).setTextSize(1);
oled.print("Temperature: ");
oled.print(25);
oled.println(" C");
oled.show();
```

## Icons

Available icons:

- `ZOROZ_ICON_HEART`
- `ZOROZ_ICON_BELL`
- `ZOROZ_ICON_CHECK`
- `ZOROZ_ICON_CROSS`
- `ZOROZ_ICON_PLAY`
- `ZOROZ_ICON_WIFI`
- `ZOROZ_ICON_BATTERY`
- `ZOROZ_ICON_WARNING`
- `ZOROZ_ICON_INFO`

Example:

```cpp
oled.clear();
oled.drawIcon(ZOROZ_ICON_BELL, 4, 4);
oled.drawIcon(ZOROZ_ICON_HEART, 32, 4);
oled.drawIcon(ZOROZ_ICON_PLAY, 60, 4);
oled.show();
```

## Widgets

```cpp
oled.wifi(75, 5, 5, 1);
oled.battery(80, 95, 5, 28, 10);
oled.progressBar(60, 4, 45, 120, 14, true);
oled.show();
```

## Animations

```cpp
oled.splash("ENGR ZOROZ");
oled.typewriter("Welcome to ZOROZ OLED");
oled.spinner(1500);
oled.animateProgress(100);
oled.scrollText("EASY OLED LIBRARY");
oled.wipe();
```

Version 1.0.0 ki animation functions blocking hain. Motor control, communication ya safety-critical loop mein long animations use na karo.

## Full Adafruit GFX access

Advanced drawing ke liye:

```cpp
Adafruit_GFX *canvas = oled.gfx();
canvas->drawCircle(64, 32, 20, ZOROZ_WHITE);
canvas->drawLine(0, 0, 127, 63, ZOROZ_WHITE);
oled.show();
```

## Blank display troubleshooting

1. I2C Scanner example run karo.
2. `0x3C` aur `0x3D` check karo.
3. SDA/SCL wiring verify karo.
4. Correct controller model select karo.
5. Dependencies install verify karo.
6. Module voltage specification check karo.
7. GND common rakho.

## GitHub par publish karne se pehle

`library.properties` mein replace karo:

```properties
maintainer=Engr Zoroz <YOUR_REAL_EMAIL>
url=https://github.com/YOUR_GITHUB_USERNAME/ZOROZ_OLED
```

`library.json` mein bhi GitHub username replace karo.

Har example ko real hardware par test karo. Kam az kam SH1106, SSD1306, Uno/Nano, Mega aur ESP32 test matrix complete karo.

## GitHub release

1. Public repository `ZOROZ_OLED` banao.
2. Library ki files repository root mein upload karo.
3. Tag `1.0.0` create karo.
4. Release title `ZOROZ OLED 1.0.0` rakho.
5. GitHub Actions ke lint aur compile checks green hone do.
6. Arduino Library Registry ke `repositories.txt` mein repository URL add karke pull request open karo.

Future bug fix ke liye `1.0.1`, new backward-compatible feature ke liye `1.1.0`, aur breaking change ke liye `2.0.0` use karo.
