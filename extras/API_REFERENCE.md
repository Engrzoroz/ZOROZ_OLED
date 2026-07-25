# ZOROZ OLED API Reference

## Constructor

```cpp
ZorozOLED oled(model, address, resetPin, wire, i2cClock, restoreClock);
```

Defaults:

- `model`: `ZOROZ_OLED_1_3`
- `address`: `0x3C`
- `resetPin`: `-1`
- `wire`: `&Wire`
- `i2cClock`: `400000`
- `restoreClock`: `100000`

Models:

- `ZOROZ_OLED_1_3` / `ZOROZ_SH1106_128X64`
- `ZOROZ_OLED_0_96` / `ZOROZ_SSD1306_128X64`
- `ZOROZ_SSD1306_128X32`

## Initialisation

```cpp
bool begin(bool resetDisplay = true, bool clearOnStart = true);
bool isReady() const;
bool isConnected();
```

Always check `begin()` in production code.

## Buffer and display

```cpp
void clear(bool showNow = false);
void fill(bool showNow = false);
void show();
```

Drawing normally updates the memory buffer. `show()` transfers the complete buffer to the OLED.

## Hardware controls

```cpp
void invert(bool enabled = true);
void setContrast(uint8_t level);
void sleep();
void wake();
void setRotation(uint8_t rotation);
```

Rotation values are `0`, `1`, `2`, and `3`.

## Arduino Print API

```cpp
oled.setCursor(0, 0);
oled.setTextSize(1);
oled.print("Value: ");
oled.println(value);
oled.show();
```

Chainable settings:

```cpp
setCursor(x, y)
setTextSize(size)
setTextColor(colour, background)
setTextWrap(enabled)
setFont(font)
```

## Text helpers

```cpp
void text(text, x, y, size, showNow);
void centeredText(text, y, size, clearLine, showNow);
void printLine(text, line, alignment, size, clearLine, showNow);
void wrappedText(text, x, y, maxWidth, size, alignment, showNow);
void showText(text, size, alignment);
void showMessage(title, message, icon);
```

Alignments:

- `ZOROZ_ALIGN_LEFT`
- `ZOROZ_ALIGN_CENTER`
- `ZOROZ_ALIGN_RIGHT`

`printLine()` uses an 8-pixel classic-font line height multiplied by `size`.

## Graphics helpers

```cpp
pixel(x, y, colour)
line(x0, y0, x1, y1, colour)
rect(x, y, width, height, colour)
fillRect(x, y, width, height, colour)
circle(x, y, radius, colour)
fillCircle(x, y, radius, colour)
roundRect(x, y, width, height, radius, colour)
fillRoundRect(x, y, width, height, radius, colour)
triangle(x0, y0, x1, y1, x2, y2, colour)
fillTriangle(x0, y0, x1, y1, x2, y2, colour)
bitmap(x, y, data, width, height, colour)
```

Colours:

- `ZOROZ_BLACK`
- `ZOROZ_WHITE`
- `ZOROZ_INVERSE`

## Icons and widgets

```cpp
void drawIcon(icon, x, y, size, showNow);
void progressBar(percent, x, y, width, height, showPercent, showNow);
void battery(percent, x, y, width, height, charging, showNow);
void wifi(strength, x, y, size, showNow);
```

Icons:

- `ZOROZ_ICON_HEART`
- `ZOROZ_ICON_BELL`
- `ZOROZ_ICON_CHECK`
- `ZOROZ_ICON_CROSS`
- `ZOROZ_ICON_PLAY`
- `ZOROZ_ICON_WIFI`
- `ZOROZ_ICON_BATTERY`
- `ZOROZ_ICON_WARNING`
- `ZOROZ_ICON_INFO`

## Animations

```cpp
void splash(name, durationMs);
void typewriter(text, x, y, size, characterDelayMs, clearFirst);
void scrollText(text, y, size, frameDelayMs, loops);
void spinner(durationMs, centerX, centerY, radius, frameDelayMs);
void animateProgress(targetPercent, frameDelayMs, x, y, width, height);
void wipe(leftToRight, frameDelayMs);
```

All version 1.0.0 animation helpers are blocking. Do not use them inside timing-critical motor, communication or safety-control loops.

## Advanced access

```cpp
Adafruit_GFX *canvas = oled.gfx();
```

This provides access to Adafruit GFX primitives, custom fonts and other advanced features.
