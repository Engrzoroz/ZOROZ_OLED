# ZOROZ OLED

**ZOROZ OLED** is a beginner-friendly high-level Arduino library for common monochrome I2C OLED displays. It gives simple functions for text, layouts, icons, progress bars and animations while preserving access to the complete Adafruit GFX drawing system.

## Supported displays

- Common 1.3 inch 128x64 SH1106 display: `ZOROZ_OLED_1_3`
- Common 0.96 inch 128x64 SSD1306 display: `ZOROZ_OLED_0_96`
- SSD1306 128x32 display: `ZOROZ_SSD1306_128X32`
- Explicit aliases: `ZOROZ_SH1106_128X64` and `ZOROZ_SSD1306_128X64`

Display size alone does not guarantee the controller type. Check the seller's description or test the model-specific options if your screen remains blank.

## Why this design is reliable

ZOROZ OLED is a high-level convenience layer. It uses the established Adafruit SH110X, Adafruit SSD1306 and Adafruit GFX libraries for low-level communication, framebuffer management and graphics. This avoids duplicating mature driver code while making normal sketches much shorter.

## Installation

### Arduino IDE ZIP installation

1. Install these libraries using **Library Manager**:
   - Adafruit GFX Library
   - Adafruit SH110X
   - Adafruit SSD1306
2. In Arduino IDE, select **Sketch > Include Library > Add .ZIP Library**.
3. Select the `ZOROZ_OLED.zip` file.
4. Open **File > Examples > ZOROZ OLED**.

Once ZOROZ OLED is accepted into Arduino Library Manager, its declared dependencies can be installed with it.

## Wiring

| OLED pin | Arduino Uno/Nano | Arduino Mega | Typical ESP32 |
|---|---:|---:|---:|
| GND | GND | GND | GND |
| VCC | 5V or 3.3V according to module | 5V or 3.3V according to module | 3.3V |
| SDA | A4 | 20 | GPIO 21 |
| SCL | A5 | 21 | GPIO 22 |

ESP32 and ESP32-S3 boards can use different default I2C pins. Use the pins printed for your exact board and initialise its `Wire` bus as required before `oled.begin()`.

## Fastest possible code

### 1.3 inch SH1106

```cpp
#include <ZorozOLED.h>

ZorozOLED oled(ZOROZ_OLED_1_3);

void setup() {
  oled.begin();
  oled.showText("HELLO", 2);
}

void loop() {
}
```

### 0.96 inch SSD1306

```cpp
#include <ZorozOLED.h>

ZorozOLED oled(ZOROZ_OLED_0_96);

void setup() {
  oled.begin();
  oled.showMessage("ZOROZ OLED", "Display is ready!", ZOROZ_ICON_CHECK);
}

void loop() {
}
```

## Non-default I2C address

```cpp
ZorozOLED oled(ZOROZ_OLED_0_96, 0x3D);
```

## Basic framebuffer workflow

Most drawing functions modify the memory buffer. Call `show()` after building the complete frame:

```cpp
oled.clear();
oled.centeredText("ENGR ZOROZ", 2, 1);
oled.drawIcon(ZOROZ_ICON_HEART, 56, 20);
oled.progressBar(75, 4, 45, 120, 12, true);
oled.show();
```

This approach is faster and produces less flicker than refreshing after every drawing operation.

## Easy functions

### Text

```cpp
oled.showText("HELLO", 2);
oled.centeredText("CENTER", 20, 1);
oled.printLine("RIGHT", 4, ZOROZ_ALIGN_RIGHT, 1);
oled.wrappedText("A longer message that wraps automatically", 2, 16, 124);
```

### Status widgets

```cpp
oled.drawIcon(ZOROZ_ICON_BELL, 4, 2);
oled.wifi(75, 30, 2);
oled.battery(85, 96, 4, 28, 10);
oled.progressBar(60, 4, 45, 120, 14, true);
oled.show();
```

### Animations

```cpp
oled.splash("ENGR ZOROZ");
oled.typewriter("Welcome to ZOROZ OLED");
oled.spinner(1500);
oled.animateProgress(100);
oled.scrollText("EASY OLED LIBRARY");
```

These animation helpers are blocking by design in version 1.0.0. They are intended for intros, loading screens and short effects.

## Standard `print()` support

ZorozOLED inherits from Arduino `Print`, so familiar code works:

```cpp
oled.clear();
oled.setTextSize(1).setCursor(0, 0);
oled.print("Temperature: ");
oled.print(25);
oled.println(" C");
oled.show();
```

## Advanced Adafruit GFX access

```cpp
Adafruit_GFX *canvas = oled.gfx();
canvas->drawCircle(64, 32, 20, ZOROZ_WHITE);
canvas->drawLine(0, 0, 127, 63, ZOROZ_WHITE);
oled.show();
```

## API overview

- Setup: `begin`, `isReady`, `isConnected`
- Buffer: `clear`, `fill`, `show`
- Display control: `invert`, `setContrast`, `sleep`, `wake`, `setRotation`
- Text: `text`, `centeredText`, `printLine`, `wrappedText`, `showText`, `showMessage`
- Graphics: `pixel`, `line`, `rect`, `fillRect`, `circle`, `fillCircle`, `roundRect`, `triangle`, `bitmap`
- Widgets: `drawIcon`, `progressBar`, `battery`, `wifi`
- Animations: `splash`, `typewriter`, `scrollText`, `spinner`, `animateProgress`, `wipe`
- Advanced: `gfx`

See `extras/API_REFERENCE.md` for parameters and usage notes. A complete Roman Urdu guide is included at `extras/ROMAN_URDU_GUIDE.md`.

## Memory note

A 128x64 monochrome framebuffer requires about 1024 bytes of RAM. Arduino Uno and Nano only have 2048 bytes of SRAM, so avoid large global arrays and excessive `String` use in memory-heavy sketches. ESP32 and similar boards have much more RAM.

## Troubleshooting

### Blank display

- Select the correct controller option.
- Try I2C address `0x3C`, then `0x3D`.
- Check SDA and SCL are not swapped.
- Check the module voltage requirement.
- Run an I2C scanner.
- Confirm all dependencies are installed.

### Compilation says a header is missing

Install all three dependencies through Arduino Library Manager:

- Adafruit GFX Library
- Adafruit SH110X
- Adafruit SSD1306

### Display is shifted or noisy

Confirm the screen really uses SH1106 or SSD1306 and is 128 pixels wide. Some visually similar modules use different controller chips.

## Publishing preparation

Before submitting to Arduino Library Manager, replace these placeholders:

- `YOUR_EMAIL@example.com`
- `YOUR_GITHUB_USERNAME`

Then test every example, create a GitHub release/tag matching `version=1.0.0`, and submit the repository URL to Arduino's Library Registry.

## Licence

ZOROZ OLED is released under the MIT License. Its external Adafruit dependencies retain their own licences and copyrights.
