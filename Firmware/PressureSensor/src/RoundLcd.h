#ifndef ROUND_LCD_H
#define ROUND_LCD_H

#include <Arduino.h>
#include <Arduino_GFX_Library.h>

class RoundLcd {
public:
  RoundLcd(int8_t dcPin, int8_t csPin, int8_t rstPin, int8_t backlightPin);

  bool begin();
  Arduino_GFX *display();

private:
  int8_t _backlightPin;
  Arduino_DataBus *_bus;
  Arduino_GFX *_display;
};

#endif
