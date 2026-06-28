#include "RoundLcd.h"

static const int16_t LCD_WIDTH = 240;
static const int16_t LCD_HEIGHT = 240;

RoundLcd::RoundLcd(int8_t dcPin, int8_t csPin, int8_t rstPin, int8_t backlightPin)
    : _backlightPin(backlightPin),
      _bus(new Arduino_HWSPI(dcPin, csPin)),
      _display(new Arduino_GC9A01(_bus, rstPin, 0, true, LCD_WIDTH, LCD_HEIGHT)) {
}

bool RoundLcd::begin() {
  pinMode(_backlightPin, OUTPUT);
  digitalWrite(_backlightPin, HIGH);

  return _display->begin();
}

Arduino_GFX *RoundLcd::display() {
  return _display;
}
