#include "PressureDisplay.h"

static uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

static const uint16_t COLOR_BG = rgb565(10, 18, 28);
static const uint16_t COLOR_PANEL = rgb565(17, 30, 46);
static const uint16_t COLOR_GRID = rgb565(42, 68, 88);
static const uint16_t COLOR_TEXT = rgb565(235, 246, 250);
static const uint16_t COLOR_MUTED = rgb565(150, 175, 190);
static const uint16_t COLOR_ACCENT = rgb565(0, 210, 180);
static const uint16_t COLOR_AMBER = rgb565(255, 190, 70);
static const uint16_t COLOR_FAULT = rgb565(255, 78, 78);

static const char *statusText(SensorStatus status) {
  switch (status) {
  case SensorStatus::OK:
    return "OK";
  case SensorStatus::UNDER_RANGE:
    return "UNDER RANGE";
  case SensorStatus::OVER_RANGE:
    return "OVER RANGE";
  case SensorStatus::ADC_FAULT:
    return "ADC FAULT";
  case SensorStatus::SENSOR_DISCONNECTED:
    return "SENSOR LOST";
  case SensorStatus::NOT_CALIBRATED:
    return "NOT CAL";
  default:
    return "UNKNOWN";
  }
}

static uint16_t statusColor(SensorStatus status) {
  switch (status) {
  case SensorStatus::OK:
    return COLOR_ACCENT;
  case SensorStatus::UNDER_RANGE:
  case SensorStatus::OVER_RANGE:
  case SensorStatus::NOT_CALIBRATED:
    return COLOR_AMBER;
  case SensorStatus::ADC_FAULT:
  case SensorStatus::SENSOR_DISCONNECTED:
  default:
    return COLOR_FAULT;
  }
}

PressureDisplay::PressureDisplay(Arduino_GFX *display, float sensorMaxKpa)
    : _display(display), _sensorMaxKpa(sensorMaxKpa) {
}

void PressureDisplay::drawStaticPressureScreen() {
  _display->fillScreen(COLOR_BG);

  _display->fillCircle(120, 120, 114, COLOR_PANEL);
  _display->drawCircle(120, 120, 115, COLOR_ACCENT);
  _display->drawCircle(120, 120, 108, COLOR_GRID);

  _display->setTextColor(COLOR_TEXT);
  _display->setTextSize(2);
  _display->setCursor(48, 28);
  _display->print("MPX2050DP");

  _display->setTextColor(COLOR_MUTED);
  _display->setTextSize(1);
  _display->setCursor(70, 50);
  _display->print("mock pressure data");

  _display->drawRoundRect(36, 76, 168, 62, 8, COLOR_GRID);
  _display->drawRoundRect(46, 160, 148, 14, 7, COLOR_GRID);
  _display->drawRoundRect(48, 210, 144, 18, 6, COLOR_GRID);

  _display->setTextColor(COLOR_MUTED);
  _display->setCursor(53, 145);
  _display->print("cmH2O");
  _display->setCursor(153, 145);
  _display->print("%FS");
}

void PressureDisplay::drawPressureData(const PressureReading &reading) {
  const float pressureKpa = reading.pressureKpa;
  const float cmH2O = pressureKpa * 10.1972f;
  const int percent = constrain(round((pressureKpa / _sensorMaxKpa) * 100.0f), 0, 100);
  const int barWidth = map(percent, 0, 100, 0, 144);
  const uint16_t activeColor = statusColor(reading.status);

  _display->fillRect(48, 88, 144, 36, COLOR_PANEL);
  _display->setTextColor(activeColor);
  _display->setTextSize(4);
  _display->setCursor(48, 88);
  _display->print(pressureKpa, 1);

  _display->setTextColor(COLOR_TEXT);
  _display->setTextSize(1);
  _display->setCursor(157, 112);
  _display->print("kPa");

  _display->fillRect(45, 160, 150, 16, COLOR_PANEL);
  _display->fillRoundRect(48, 163, barWidth, 8, 4, activeColor);

  _display->fillRect(38, 185, 166, 18, COLOR_PANEL);
  _display->setTextColor(COLOR_TEXT);
  _display->setTextSize(2);
  _display->setCursor(39, 186);
  _display->print(cmH2O, 0);

  _display->setCursor(150, 186);
  _display->print(percent);
  _display->print("%");

  _display->fillRect(51, 213, 138, 12, COLOR_PANEL);
  _display->setTextColor(activeColor);
  _display->setTextSize(1);
  _display->setCursor(72, 216);
  _display->print(statusText(reading.status));
}
