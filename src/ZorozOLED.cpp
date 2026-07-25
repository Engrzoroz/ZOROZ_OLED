#include "ZorozOLED.h"

ZorozOLED::ZorozOLED(ZorozOLEDModel model, uint8_t address,
                     int8_t resetPin, TwoWire *wire,
                     uint32_t i2cClock, uint32_t restoreClock)
    : _model(model), _address(address), _resetPin(resetPin),
      _wire(wire ? wire : &Wire), _i2cClock(i2cClock),
      _restoreClock(restoreClock), _ready(false), _sh1106(NULL),
      _ssd1306(NULL), _gfx(NULL), _textColor(ZOROZ_WHITE),
      _textBackground(ZOROZ_BLACK), _textSize(1) {}

ZorozOLED::~ZorozOLED() {
  releaseDriver();
}

int16_t ZorozOLED::rawWidth() const {
  return 128;
}

int16_t ZorozOLED::rawHeight() const {
  return (_model == ZOROZ_SSD1306_128X32) ? 32 : 64;
}

void ZorozOLED::releaseDriver() {
  _gfx = NULL;
  if (_sh1106) {
    delete _sh1106;
    _sh1106 = NULL;
  }
  if (_ssd1306) {
    delete _ssd1306;
    _ssd1306 = NULL;
  }
  _ready = false;
}

bool ZorozOLED::begin(bool resetDisplay, bool clearOnStart) {
  releaseDriver();

  bool ok = false;
  if (_model == ZOROZ_OLED_1_3) {
    _sh1106 = new Adafruit_SH1106G(rawWidth(), rawHeight(), _wire,
                                   _resetPin, _i2cClock, _restoreClock);
    if (!_sh1106) {
      return false;
    }
    _gfx = _sh1106;
    ok = _sh1106->begin(_address, resetDisplay);
  } else {
    _ssd1306 = new Adafruit_SSD1306(rawWidth(), rawHeight(), _wire,
                                    _resetPin, _i2cClock, _restoreClock);
    if (!_ssd1306) {
      return false;
    }
    _gfx = _ssd1306;
    ok = _ssd1306->begin(SSD1306_SWITCHCAPVCC, _address,
                         resetDisplay, true);
  }

  if (!ok) {
    releaseDriver();
    return false;
  }

  _ready = true;
  _gfx->setRotation(0);
  _gfx->setTextColor(_textColor, _textBackground);
  _gfx->setTextSize(_textSize);
  _gfx->setTextWrap(true);

  if (clearOnStart) {
    clear(true);
  }

  return true;
}

bool ZorozOLED::isReady() const {
  return _ready && _gfx;
}

bool ZorozOLED::isConnected() {
  if (!_wire) {
    return false;
  }
  _wire->beginTransmission(_address);
  return _wire->endTransmission() == 0;
}

void ZorozOLED::show() {
  if (!_ready) {
    return;
  }
  if (_model == ZOROZ_OLED_1_3) {
    _sh1106->display();
  } else {
    _ssd1306->display();
  }
}

void ZorozOLED::clear(bool showNow) {
  if (!_ready) {
    return;
  }
  if (_model == ZOROZ_OLED_1_3) {
    _sh1106->clearDisplay();
  } else {
    _ssd1306->clearDisplay();
  }
  pushIf(showNow);
}

void ZorozOLED::fill(bool showNow) {
  if (!_gfx) {
    return;
  }
  _gfx->fillScreen(ZOROZ_WHITE);
  pushIf(showNow);
}

int16_t ZorozOLED::width() const {
  return _gfx ? _gfx->width() : rawWidth();
}

int16_t ZorozOLED::height() const {
  return _gfx ? _gfx->height() : rawHeight();
}

uint8_t ZorozOLED::address() const {
  return _address;
}

ZorozOLEDModel ZorozOLED::model() const {
  return _model;
}

void ZorozOLED::setRotation(uint8_t rotation) {
  if (_gfx) {
    _gfx->setRotation(rotation & 3);
  }
}

uint8_t ZorozOLED::getRotation() const {
  return _gfx ? _gfx->getRotation() : 0;
}

void ZorozOLED::invert(bool enabled) {
  if (!_ready) {
    return;
  }
  if (_model == ZOROZ_OLED_1_3) {
    _sh1106->invertDisplay(enabled);
  } else {
    _ssd1306->invertDisplay(enabled);
  }
}

void ZorozOLED::setContrast(uint8_t level) {
  if (!_ready) {
    return;
  }
  if (_model == ZOROZ_OLED_1_3) {
    _sh1106->setContrast(level);
  } else {
    _ssd1306->ssd1306_command(SSD1306_SETCONTRAST);
    _ssd1306->ssd1306_command(level);
  }
}

void ZorozOLED::command(uint8_t value) {
  if (!_ready) {
    return;
  }
  if (_model == ZOROZ_OLED_1_3) {
    _sh1106->oled_command(value);
  } else {
    _ssd1306->ssd1306_command(value);
  }
}

void ZorozOLED::sleep() {
  command((_model == ZOROZ_OLED_1_3) ? SH110X_DISPLAYOFF
                                      : SSD1306_DISPLAYOFF);
}

void ZorozOLED::wake() {
  command((_model == ZOROZ_OLED_1_3) ? SH110X_DISPLAYON
                                      : SSD1306_DISPLAYON);
}

size_t ZorozOLED::write(uint8_t value) {
  return _gfx ? _gfx->write(value) : 0;
}

ZorozOLED &ZorozOLED::setCursor(int16_t x, int16_t y) {
  if (_gfx) {
    _gfx->setCursor(x, y);
  }
  return *this;
}

ZorozOLED &ZorozOLED::setTextSize(uint8_t size) {
  _textSize = size ? size : 1;
  if (_gfx) {
    _gfx->setTextSize(_textSize);
  }
  return *this;
}

ZorozOLED &ZorozOLED::setTextColor(uint16_t color, uint16_t background) {
  _textColor = color;
  _textBackground = background;
  if (_gfx) {
    _gfx->setTextColor(color, background);
  }
  return *this;
}

ZorozOLED &ZorozOLED::setTextWrap(bool enabled) {
  if (_gfx) {
    _gfx->setTextWrap(enabled);
  }
  return *this;
}

ZorozOLED &ZorozOLED::setFont(const GFXfont *font) {
  if (_gfx) {
    _gfx->setFont(font);
  }
  return *this;
}

int16_t ZorozOLED::textWidth(const String &text) {
  if (!_gfx) {
    return 0;
  }
  int16_t x1, y1;
  uint16_t w, h;
  _gfx->getTextBounds(text.c_str(), 0, 0, &x1, &y1, &w, &h);
  return (int16_t)w;
}

int16_t ZorozOLED::textHeight(const String &text) {
  if (!_gfx) {
    return 0;
  }
  int16_t x1, y1;
  uint16_t w, h;
  _gfx->getTextBounds(text.c_str(), 0, 0, &x1, &y1, &w, &h);
  return (int16_t)h;
}

int16_t ZorozOLED::alignedX(const String &text, ZorozOLEDAlign align,
                            int16_t left, int16_t areaWidth) {
  if (areaWidth < 0) {
    areaWidth = width() - left;
  }
  int16_t w = textWidth(text);
  if (align == ZOROZ_ALIGN_CENTER) {
    return left + (areaWidth - w) / 2;
  }
  if (align == ZOROZ_ALIGN_RIGHT) {
    return left + areaWidth - w;
  }
  return left;
}

void ZorozOLED::text(const String &value, int16_t x, int16_t y,
                     uint8_t size, bool showNow) {
  if (!_gfx) {
    return;
  }
  setFont(NULL);
  setTextSize(size);
  setTextColor(ZOROZ_WHITE, ZOROZ_BLACK);
  setCursor(x, y);
  _gfx->print(value);
  pushIf(showNow);
}

void ZorozOLED::centeredText(const String &value, int16_t y,
                             uint8_t size, bool clearLine,
                             bool showNow) {
  if (!_gfx) {
    return;
  }
  setFont(NULL);
  setTextSize(size);
  setTextColor(ZOROZ_WHITE, ZOROZ_BLACK);
  int16_t lineHeight = 8 * size;
  if (clearLine) {
    _gfx->fillRect(0, y, width(), lineHeight, ZOROZ_BLACK);
  }
  int16_t x = alignedX(value, ZOROZ_ALIGN_CENTER);
  setCursor(x < 0 ? 0 : x, y);
  _gfx->print(value);
  pushIf(showNow);
}

void ZorozOLED::printLine(const String &value, uint8_t lineNumber,
                          ZorozOLEDAlign align, uint8_t size,
                          bool clearLine, bool showNow) {
  if (!_gfx) {
    return;
  }
  setFont(NULL);
  setTextSize(size);
  setTextColor(ZOROZ_WHITE, ZOROZ_BLACK);
  int16_t lineHeight = 8 * size;
  int16_t y = lineNumber * lineHeight;
  if (y >= height()) {
    return;
  }
  if (clearLine) {
    _gfx->fillRect(0, y, width(), lineHeight, ZOROZ_BLACK);
  }
  int16_t x = alignedX(value, align);
  setCursor(x < 0 ? 0 : x, y);
  _gfx->print(value);
  pushIf(showNow);
}

void ZorozOLED::wrappedText(const String &value, int16_t x, int16_t y,
                            int16_t maxWidth, uint8_t size,
                            ZorozOLEDAlign align, bool showNow) {
  if (!_gfx || maxWidth <= 0) {
    return;
  }

  setFont(NULL);
  setTextSize(size);
  setTextColor(ZOROZ_WHITE, ZOROZ_BLACK);
  setTextWrap(false);

  const int16_t lineHeight = 8 * size;
  String lineValue;
  String word;
  int16_t currentY = y;

  for (uint16_t i = 0; i <= value.length(); ++i) {
    char c = (i < value.length()) ? value[i] : ' ';
    bool forcedBreak = (c == '\n');
    bool wordEnd = (c == ' ' || c == '\t' || forcedBreak || i == value.length());

    if (!wordEnd) {
      word += c;
      continue;
    }

    if (word.length()) {
      String candidate = lineValue.length() ? lineValue + " " + word : word;
      if (textWidth(candidate) > maxWidth && lineValue.length()) {
        int16_t drawX = alignedX(lineValue, align, x, maxWidth);
        setCursor(drawX < x ? x : drawX, currentY);
        _gfx->print(lineValue);
        currentY += lineHeight;
        lineValue = word;
      } else {
        lineValue = candidate;
      }
      word = "";
    }

    if (forcedBreak) {
      if (lineValue.length()) {
        int16_t drawX = alignedX(lineValue, align, x, maxWidth);
        setCursor(drawX < x ? x : drawX, currentY);
        _gfx->print(lineValue);
        lineValue = "";
      }
      currentY += lineHeight;
    }

    if (currentY + lineHeight > height()) {
      break;
    }
  }

  if (lineValue.length() && currentY < height()) {
    int16_t drawX = alignedX(lineValue, align, x, maxWidth);
    setCursor(drawX < x ? x : drawX, currentY);
    _gfx->print(lineValue);
  }

  setTextWrap(true);
  pushIf(showNow);
}

void ZorozOLED::showText(const String &value, uint8_t size,
                         ZorozOLEDAlign align) {
  if (!_gfx) {
    return;
  }
  clear(false);
  setFont(NULL);
  setTextSize(size);
  setTextColor(ZOROZ_WHITE, ZOROZ_BLACK);
  int16_t h = textHeight(value);
  int16_t y = (height() - h) / 2;
  int16_t x = alignedX(value, align);
  if (x < 0) {
    x = 0;
  }
  if (y < 0) {
    y = 0;
  }
  setCursor(x, y);
  _gfx->print(value);
  show();
}

void ZorozOLED::showMessage(const String &title, const String &message,
                            ZorozOLEDIcon icon) {
  if (!_gfx) {
    return;
  }
  clear(false);
  drawIcon(icon, 2, 1, 1, false);
  setFont(NULL);
  setTextSize(1);
  setTextColor(ZOROZ_WHITE, ZOROZ_BLACK);
  int16_t titleX = alignedX(title, ZOROZ_ALIGN_CENTER, 18, width() - 20);
  setCursor(titleX < 18 ? 18 : titleX, 2);
  _gfx->print(title);
  _gfx->drawFastHLine(0, 12, width(), ZOROZ_WHITE);
  wrappedText(message, 2, 17, width() - 4, 1,
              ZOROZ_ALIGN_CENTER, false);
  show();
}

void ZorozOLED::pixel(int16_t x, int16_t y, uint16_t color) {
  if (_gfx) {
    _gfx->drawPixel(x, y, color);
  }
}

void ZorozOLED::line(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                     uint16_t color) {
  if (_gfx) {
    _gfx->drawLine(x0, y0, x1, y1, color);
  }
}

void ZorozOLED::rect(int16_t x, int16_t y, int16_t w, int16_t h,
                     uint16_t color) {
  if (_gfx) {
    _gfx->drawRect(x, y, w, h, color);
  }
}

void ZorozOLED::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                         uint16_t color) {
  if (_gfx) {
    _gfx->fillRect(x, y, w, h, color);
  }
}

void ZorozOLED::circle(int16_t x, int16_t y, int16_t radius,
                       uint16_t color) {
  if (_gfx) {
    _gfx->drawCircle(x, y, radius, color);
  }
}

void ZorozOLED::fillCircle(int16_t x, int16_t y, int16_t radius,
                           uint16_t color) {
  if (_gfx) {
    _gfx->fillCircle(x, y, radius, color);
  }
}

void ZorozOLED::roundRect(int16_t x, int16_t y, int16_t w, int16_t h,
                          int16_t radius, uint16_t color) {
  if (_gfx) {
    _gfx->drawRoundRect(x, y, w, h, radius, color);
  }
}

void ZorozOLED::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
                              int16_t radius, uint16_t color) {
  if (_gfx) {
    _gfx->fillRoundRect(x, y, w, h, radius, color);
  }
}

void ZorozOLED::triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                         int16_t x2, int16_t y2, uint16_t color) {
  if (_gfx) {
    _gfx->drawTriangle(x0, y0, x1, y1, x2, y2, color);
  }
}

void ZorozOLED::fillTriangle(int16_t x0, int16_t y0, int16_t x1,
                             int16_t y1, int16_t x2, int16_t y2,
                             uint16_t color) {
  if (_gfx) {
    _gfx->fillTriangle(x0, y0, x1, y1, x2, y2, color);
  }
}

void ZorozOLED::bitmap(int16_t x, int16_t y, const uint8_t *data,
                       int16_t w, int16_t h, uint16_t color) {
  if (_gfx && data) {
    _gfx->drawBitmap(x, y, data, w, h, color);
  }
}

void ZorozOLED::drawHeart(int16_t x, int16_t y, uint8_t size) {
  int16_t s = size;
  _gfx->fillCircle(x + 4 * s, y + 4 * s, 3 * s, ZOROZ_WHITE);
  _gfx->fillCircle(x + 10 * s, y + 4 * s, 3 * s, ZOROZ_WHITE);
  _gfx->fillTriangle(x + 1 * s, y + 5 * s,
                     x + 13 * s, y + 5 * s,
                     x + 7 * s, y + 13 * s, ZOROZ_WHITE);
}

void ZorozOLED::drawBell(int16_t x, int16_t y, uint8_t size) {
  int16_t s = size;
  _gfx->fillCircle(x + 7 * s, y + 3 * s, 2 * s, ZOROZ_WHITE);
  _gfx->fillRoundRect(x + 3 * s, y + 3 * s, 8 * s, 8 * s,
                      3 * s, ZOROZ_WHITE);
  _gfx->fillRect(x + 1 * s, y + 10 * s, 12 * s, 2 * s, ZOROZ_WHITE);
  _gfx->fillCircle(x + 7 * s, y + 13 * s, 1 * s, ZOROZ_WHITE);
}

void ZorozOLED::drawCheck(int16_t x, int16_t y, uint8_t size) {
  int16_t s = size;
  for (int16_t i = 0; i < s + 1; ++i) {
    _gfx->drawLine(x + i, y + 7 * s,
                   x + 5 * s + i, y + 12 * s, ZOROZ_WHITE);
    _gfx->drawLine(x + 5 * s + i, y + 12 * s,
                   x + 14 * s + i, y + 2 * s, ZOROZ_WHITE);
  }
}

void ZorozOLED::drawCross(int16_t x, int16_t y, uint8_t size) {
  int16_t s = size;
  for (int16_t i = 0; i < s + 1; ++i) {
    _gfx->drawLine(x + 2 * s + i, y + 2 * s,
                   x + 12 * s + i, y + 12 * s, ZOROZ_WHITE);
    _gfx->drawLine(x + 12 * s + i, y + 2 * s,
                   x + 2 * s + i, y + 12 * s, ZOROZ_WHITE);
  }
}

void ZorozOLED::drawPlay(int16_t x, int16_t y, uint8_t size) {
  int16_t s = size;
  _gfx->fillTriangle(x + 3 * s, y + 1 * s,
                     x + 3 * s, y + 13 * s,
                     x + 13 * s, y + 7 * s, ZOROZ_WHITE);
}

void ZorozOLED::drawWarning(int16_t x, int16_t y, uint8_t size) {
  int16_t s = size;
  _gfx->drawTriangle(x + 7 * s, y,
                     x, y + 14 * s,
                     x + 14 * s, y + 14 * s, ZOROZ_WHITE);
  _gfx->fillRect(x + 6 * s, y + 5 * s, 2 * s, 5 * s, ZOROZ_WHITE);
  _gfx->fillRect(x + 6 * s, y + 12 * s, 2 * s, 2 * s, ZOROZ_WHITE);
}

void ZorozOLED::drawInfo(int16_t x, int16_t y, uint8_t size) {
  int16_t s = size;
  _gfx->drawCircle(x + 7 * s, y + 7 * s, 7 * s, ZOROZ_WHITE);
  _gfx->fillCircle(x + 7 * s, y + 4 * s, s, ZOROZ_WHITE);
  _gfx->fillRect(x + 6 * s, y + 7 * s, 2 * s, 5 * s, ZOROZ_WHITE);
}

void ZorozOLED::drawIcon(ZorozOLEDIcon icon, int16_t x, int16_t y,
                         uint8_t size, bool showNow) {
  if (!_gfx) {
    return;
  }
  if (size == 0) {
    size = 1;
  }
  switch (icon) {
    case ZOROZ_ICON_HEART:
      drawHeart(x, y, size);
      break;
    case ZOROZ_ICON_BELL:
      drawBell(x, y, size);
      break;
    case ZOROZ_ICON_CHECK:
      drawCheck(x, y, size);
      break;
    case ZOROZ_ICON_CROSS:
      drawCross(x, y, size);
      break;
    case ZOROZ_ICON_PLAY:
      drawPlay(x, y, size);
      break;
    case ZOROZ_ICON_WIFI:
      wifi(100, x, y, size, false);
      break;
    case ZOROZ_ICON_BATTERY:
      battery(75, x, y + 2 * size, 16 * size, 8 * size, false, false);
      break;
    case ZOROZ_ICON_WARNING:
      drawWarning(x, y, size);
      break;
    case ZOROZ_ICON_INFO:
    default:
      drawInfo(x, y, size);
      break;
  }
  pushIf(showNow);
}

void ZorozOLED::progressBar(uint8_t percent, int16_t x, int16_t y,
                            int16_t w, int16_t h, bool showPercent,
                            bool showNow) {
  if (!_gfx || w < 6 || h < 5) {
    return;
  }
  if (percent > 100) {
    percent = 100;
  }

  _gfx->fillRect(x, y, w, h, ZOROZ_BLACK);
  _gfx->drawRoundRect(x, y, w, h, 2, ZOROZ_WHITE);
  int16_t innerWidth = w - 4;
  int16_t fillWidth = ((int32_t)innerWidth * percent) / 100;
  if (fillWidth > 0) {
    _gfx->fillRect(x + 2, y + 2, fillWidth, h - 4, ZOROZ_WHITE);
  }

  if (showPercent && h >= 11) {
    String label = String(percent) + "%";
    setFont(NULL);
    setTextSize(1);
    int16_t tx = alignedX(label, ZOROZ_ALIGN_CENTER, x, w);
    int16_t ty = y + (h - 8) / 2;
    uint16_t textColour = (percent >= 50) ? ZOROZ_BLACK : ZOROZ_WHITE;
    uint16_t bgColour = (percent >= 50) ? ZOROZ_WHITE : ZOROZ_BLACK;
    _gfx->setTextColor(textColour, bgColour);
    _gfx->setCursor(tx, ty);
    _gfx->print(label);
    _gfx->setTextColor(_textColor, _textBackground);
  }

  pushIf(showNow);
}

void ZorozOLED::battery(uint8_t percent, int16_t x, int16_t y,
                        int16_t w, int16_t h, bool charging,
                        bool showNow) {
  if (!_gfx || w < 8 || h < 6) {
    return;
  }
  if (percent > 100) {
    percent = 100;
  }
  int16_t terminal = (w / 8 > 2) ? (w / 8) : 2;
  _gfx->drawRect(x, y, w - terminal - 1, h, ZOROZ_WHITE);
  _gfx->fillRect(x + w - terminal - 1, y + h / 3,
                 terminal, (h / 3 > 2) ? (h / 3) : 2, ZOROZ_WHITE);
  int16_t insideWidth = w - terminal - 5;
  int16_t chargeWidth = ((int32_t)insideWidth * percent) / 100;
  if (chargeWidth > 0) {
    _gfx->fillRect(x + 2, y + 2, chargeWidth, h - 4, ZOROZ_WHITE);
  }
  if (charging) {
    int16_t cx = x + (w - terminal) / 2;
    int16_t cy = y + h / 2;
    _gfx->drawLine(cx + 1, cy - h / 3, cx - 2, cy, ZOROZ_INVERSE);
    _gfx->drawLine(cx - 2, cy, cx + 1, cy, ZOROZ_INVERSE);
    _gfx->drawLine(cx + 1, cy, cx - 1, cy + h / 3, ZOROZ_INVERSE);
  }
  pushIf(showNow);
}

void ZorozOLED::wifi(uint8_t strength, int16_t x, int16_t y,
                     uint8_t size, bool showNow) {
  if (!_gfx) {
    return;
  }
  if (strength > 100) {
    strength = 100;
  }
  if (size == 0) {
    size = 1;
  }
  int16_t s = size;
  _gfx->fillCircle(x + 7 * s, y + 12 * s, s, ZOROZ_WHITE);
  if (strength >= 25) {
    _gfx->drawLine(x + 4 * s, y + 9 * s,
                   x + 7 * s, y + 6 * s, ZOROZ_WHITE);
    _gfx->drawLine(x + 7 * s, y + 6 * s,
                   x + 10 * s, y + 9 * s, ZOROZ_WHITE);
  }
  if (strength >= 50) {
    _gfx->drawLine(x + 2 * s, y + 7 * s,
                   x + 7 * s, y + 2 * s, ZOROZ_WHITE);
    _gfx->drawLine(x + 7 * s, y + 2 * s,
                   x + 12 * s, y + 7 * s, ZOROZ_WHITE);
  }
  if (strength >= 75) {
    _gfx->drawLine(x, y + 5 * s,
                   x + 7 * s, y, ZOROZ_WHITE);
    _gfx->drawLine(x + 7 * s, y,
                   x + 14 * s, y + 5 * s, ZOROZ_WHITE);
  }
  pushIf(showNow);
}

void ZorozOLED::splash(const String &name, uint16_t durationMs) {
  if (!_gfx) {
    return;
  }
  clear(false);
  _gfx->drawRoundRect(1, 1, width() - 2, height() - 2, 6, ZOROZ_WHITE);
  drawIcon(ZOROZ_ICON_PLAY, width() / 2 - 7, 8, 1, false);
  centeredText(name, height() / 2 - 1, 1, false, false);
  progressBar(100, 14, height() - 12, width() - 28, 7, false, false);
  show();
  delay(durationMs);
}

void ZorozOLED::typewriter(const String &value, int16_t x, int16_t y,
                           uint8_t size, uint16_t characterDelayMs,
                           bool clearFirst) {
  if (!_gfx) {
    return;
  }
  if (clearFirst) {
    clear(false);
  }
  setFont(NULL);
  setTextSize(size);
  setTextColor(ZOROZ_WHITE, ZOROZ_BLACK);
  setTextWrap(true);
  setCursor(x, y);
  for (uint16_t i = 0; i < value.length(); ++i) {
    _gfx->write((uint8_t)value[i]);
    show();
    delay(characterDelayMs);
  }
}

void ZorozOLED::scrollText(const String &value, int16_t y,
                           uint8_t size, uint16_t frameDelayMs,
                           uint8_t loops) {
  if (!_gfx || loops == 0) {
    return;
  }
  setFont(NULL);
  setTextSize(size);
  setTextColor(ZOROZ_WHITE, ZOROZ_BLACK);
  setTextWrap(false);
  int16_t valueWidth = textWidth(value);
  int16_t bandHeight = 8 * size;

  for (uint8_t loopIndex = 0; loopIndex < loops; ++loopIndex) {
    for (int16_t x = width(); x > -valueWidth; --x) {
      _gfx->fillRect(0, y, width(), bandHeight, ZOROZ_BLACK);
      _gfx->setCursor(x, y);
      _gfx->print(value);
      show();
      delay(frameDelayMs);
    }
  }
  setTextWrap(true);
}

void ZorozOLED::spinner(uint16_t durationMs, int16_t centerX,
                        int16_t centerY, uint8_t radius,
                        uint16_t frameDelayMs) {
  if (!_gfx || frameDelayMs == 0) {
    return;
  }
  if (centerX < 0) {
    centerX = width() / 2;
  }
  if (centerY < 0) {
    centerY = height() / 2;
  }
  static const int8_t dx[8] = {0, 5, 7, 5, 0, -5, -7, -5};
  static const int8_t dy[8] = {-7, -5, 0, 5, 7, 5, 0, -5};
  uint32_t started = millis();
  uint8_t frame = 0;
  int16_t clearRadius = radius + 4;

  while ((uint32_t)(millis() - started) < durationMs) {
    _gfx->fillRect(centerX - clearRadius, centerY - clearRadius,
                   clearRadius * 2 + 1, clearRadius * 2 + 1,
                   ZOROZ_BLACK);
    for (uint8_t i = 0; i < 8; ++i) {
      int16_t px = centerX + ((int16_t)dx[i] * radius) / 7;
      int16_t py = centerY + ((int16_t)dy[i] * radius) / 7;
      uint8_t dotRadius = (i == frame) ? 2 : 1;
      _gfx->fillCircle(px, py, dotRadius, ZOROZ_WHITE);
    }
    show();
    delay(frameDelayMs);
    frame = (frame + 1) & 7;
  }
}

void ZorozOLED::animateProgress(uint8_t targetPercent,
                                uint16_t frameDelayMs,
                                int16_t x, int16_t y,
                                int16_t w, int16_t h) {
  if (targetPercent > 100) {
    targetPercent = 100;
  }
  for (uint8_t percent = 0; percent <= targetPercent; ++percent) {
    progressBar(percent, x, y, w, h, true, true);
    delay(frameDelayMs);
    if (percent == 100) {
      break;
    }
  }
}

void ZorozOLED::wipe(bool leftToRight, uint16_t frameDelayMs) {
  if (!_gfx) {
    return;
  }
  if (leftToRight) {
    for (int16_t x = 0; x < width(); ++x) {
      _gfx->drawFastVLine(x, 0, height(), ZOROZ_BLACK);
      show();
      delay(frameDelayMs);
    }
  } else {
    for (int16_t x = width() - 1; x >= 0; --x) {
      _gfx->drawFastVLine(x, 0, height(), ZOROZ_BLACK);
      show();
      delay(frameDelayMs);
    }
  }
}

void ZorozOLED::pushIf(bool showNow) {
  if (showNow) {
    show();
  }
}

Adafruit_GFX *ZorozOLED::gfx() {
  return _gfx;
}

const Adafruit_GFX *ZorozOLED::gfx() const {
  return _gfx;
}
