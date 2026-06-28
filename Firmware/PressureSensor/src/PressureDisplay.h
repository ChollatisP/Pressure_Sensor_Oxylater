#ifndef PRESSURE_DISPLAY_H
#define PRESSURE_DISPLAY_H

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "PressureReading.h"

class PressureDisplay {
public:
  PressureDisplay(Arduino_GFX *display, float sensorMaxKpa);

  void drawStaticPressureScreen();
  void drawPressureData(const PressureReading &reading);

private:
  Arduino_GFX *_display;
  float _sensorMaxKpa;
};

#endif
