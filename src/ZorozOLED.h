#ifndef ZOROZ_OLED_H
#define ZOROZ_OLED_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_SSD1306.h>

#define ZOROZ_OLED_VERSION_MAJOR 1
#define ZOROZ_OLED_VERSION_MINOR 0
#define ZOROZ_OLED_VERSION_PATCH 0

#define ZOROZ_BLACK 0
#define ZOROZ_WHITE 1
#define ZOROZ_INVERSE 2

enum ZorozOLEDModel : uint8_t {
  ZOROZ_OLED_1_3 = 0,
  ZOROZ_OLED_0_96 = 1,
  ZOROZ_SSD1306_128X32 = 2
};

#define ZOROZ_SH1106_128X64 ZOROZ_OLED_1_3
#define ZOROZ_SSD1306_128X64 ZOROZ_OLED_0_96

enum ZorozOLEDAlign : uint8_t {
  ZOROZ_ALIGN_LEFT = 0,
  ZOROZ_ALIGN_CENTER = 1,
  ZOROZ_ALIGN_RIGHT = 2
};

enum ZorozOLEDIcon : uint8_t {
  ZOROZ_ICON_HEART = 0,
  ZOROZ_ICON_BELL,
  ZOROZ_ICON_CHECK,
  ZOROZ_ICON_CROSS,
  ZOROZ_ICON_PLAY,
  ZOROZ_ICON_WIFI,
  ZOROZ_ICON_BATTERY,
  ZOROZ_ICON_WARNING,
  ZOROZ_ICON_INFO
};

class ZorozOLED : public Print {
public:
  explicit ZorozOLED(
      ZorozOLEDModel model = ZOROZ_OLED_1_3,
      uint8_t address = 0x3C,
      int8_t resetPin = -1,
      TwoWire *wire = &Wire,
      uint32_t i2cClock = 400000UL,
      uint32_t restoreClock = 100000UL);

  ~ZorozOLED();

  bool begin(bool resetDisplay = true, bool clearOnStart = true);
  bool isReady() const;
  bool isConnected();

  void show();
  void clear(bool showNow = false);
  void fill(bool showNow = false);

  int16_t width() const;
  int16_t height() const;
  uint8_t address() const;
  ZorozOLEDModel model() const;

  void setRotation(uint8_t rotation);
  uint8_t getRotation() const;
  void invert(bool enabled = true);
  void setContrast(uint8_t level);
  void sleep();
  void wake();

  size_t write(uint8_t value) override;
  using Print::write;

  ZorozOLED &setCursor(int16_t x, int16_t y);
  ZorozOLED &setTextSize(uint8_t size);
  ZorozOLED &setTextColor(uint16_t color, uint16_t background = ZOROZ_BLACK);
  ZorozOLED &setTextWrap(bool enabled = true);
  ZorozOLED &setFont(const GFXfont *font = NULL);

  int16_t textWidth(const String &text);
  int16_t textHeight(const String &text);

  void text(const String &text, int16_t x, int16_t y, uint8_t size = 1,
            bool showNow = false);
  void centeredText(const String &text, int16_t y, uint8_t size = 1,
                    bool clearLine = false, bool showNow = false);
  void printLine(const String &text, uint8_t line,
                 ZorozOLEDAlign align = ZOROZ_ALIGN_LEFT,
                 uint8_t size = 1, bool clearLine = true,
                 bool showNow = false);
  void wrappedText(const String &text, int16_t x, int16_t y,
                   int16_t maxWidth, uint8_t size = 1,
                   ZorozOLEDAlign align = ZOROZ_ALIGN_LEFT,
                   bool showNow = false);
  void showText(const String &text, uint8_t size = 2,
                ZorozOLEDAlign align = ZOROZ_ALIGN_CENTER);
  void showMessage(const String &title, const String &message,
                   ZorozOLEDIcon icon = ZOROZ_ICON_INFO);

  void pixel(int16_t x, int16_t y, uint16_t color = ZOROZ_WHITE);
  void line(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
            uint16_t color = ZOROZ_WHITE);
  void rect(int16_t x, int16_t y, int16_t w, int16_t h,
            uint16_t color = ZOROZ_WHITE);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                uint16_t color = ZOROZ_WHITE);
  void circle(int16_t x, int16_t y, int16_t radius,
              uint16_t color = ZOROZ_WHITE);
  void fillCircle(int16_t x, int16_t y, int16_t radius,
                  uint16_t color = ZOROZ_WHITE);
  void roundRect(int16_t x, int16_t y, int16_t w, int16_t h,
                 int16_t radius, uint16_t color = ZOROZ_WHITE);
  void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
                     int16_t radius, uint16_t color = ZOROZ_WHITE);
  void triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                int16_t x2, int16_t y2,
                uint16_t color = ZOROZ_WHITE);
  void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                    int16_t x2, int16_t y2,
                    uint16_t color = ZOROZ_WHITE);
  void bitmap(int16_t x, int16_t y, const uint8_t *data,
              int16_t w, int16_t h, uint16_t color = ZOROZ_WHITE);

  void drawIcon(ZorozOLEDIcon icon, int16_t x, int16_t y,
                uint8_t size = 1, bool showNow = false);
  void progressBar(uint8_t percent, int16_t x = 4, int16_t y = 45,
                   int16_t w = 120, int16_t h = 12,
                   bool showPercent = true, bool showNow = false);
  void battery(uint8_t percent, int16_t x, int16_t y,
               int16_t w = 24, int16_t h = 10,
               bool charging = false, bool showNow = false);
  void wifi(uint8_t strength, int16_t x, int16_t y,
            uint8_t size = 1, bool showNow = false);

  void splash(const String &name = "ENGR ZOROZ",
              uint16_t durationMs = 1600);
  void typewriter(const String &text, int16_t x = 0, int16_t y = 0,
                  uint8_t size = 1, uint16_t characterDelayMs = 45,
                  bool clearFirst = true);
  void scrollText(const String &text, int16_t y = 24,
                  uint8_t size = 2, uint16_t frameDelayMs = 25,
                  uint8_t loops = 1);
  void spinner(uint16_t durationMs = 1200, int16_t centerX = -1,
               int16_t centerY = -1, uint8_t radius = 12,
               uint16_t frameDelayMs = 75);
  void animateProgress(uint8_t targetPercent = 100,
                       uint16_t frameDelayMs = 20,
                       int16_t x = 4, int16_t y = 45,
                       int16_t w = 120, int16_t h = 12);
  void wipe(bool leftToRight = true, uint16_t frameDelayMs = 4);

  Adafruit_GFX *gfx();
  const Adafruit_GFX *gfx() const;

private:
  ZorozOLEDModel _model;
  uint8_t _address;
  int8_t _resetPin;
  TwoWire *_wire;
  uint32_t _i2cClock;
  uint32_t _restoreClock;
  bool _ready;

  Adafruit_SH1106G *_sh1106;
  Adafruit_SSD1306 *_ssd1306;
  Adafruit_GFX *_gfx;

  uint16_t _textColor;
  uint16_t _textBackground;
  uint8_t _textSize;

  int16_t rawWidth() const;
  int16_t rawHeight() const;
  void releaseDriver();
  void pushIf(bool showNow);
  int16_t alignedX(const String &text, ZorozOLEDAlign align,
                   int16_t left = 0, int16_t areaWidth = -1);
  void command(uint8_t value);
  void drawHeart(int16_t x, int16_t y, uint8_t size);
  void drawBell(int16_t x, int16_t y, uint8_t size);
  void drawCheck(int16_t x, int16_t y, uint8_t size);
  void drawCross(int16_t x, int16_t y, uint8_t size);
  void drawPlay(int16_t x, int16_t y, uint8_t size);
  void drawWarning(int16_t x, int16_t y, uint8_t size);
  void drawInfo(int16_t x, int16_t y, uint8_t size);

  ZorozOLED(const ZorozOLED &);
  ZorozOLED &operator=(const ZorozOLED &);
};

#endif
