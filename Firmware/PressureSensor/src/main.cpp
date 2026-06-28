#include <Arduino.h>
#include "PressureDisplay.h"
#include "PressureReading.h"
#include "RoundLcd.h"

/*
  ESP-12S / ESP8266 Round LCD V1.0 pressure display mockup.

  Most 240x240 round LCD boards use a GC9A01 TFT over SPI.
  ESP-12S hardware SPI uses:
    SCK  = GPIO14
    MOSI = GPIO13
    MISO = GPIO12, not used by this display

  If your board shows a blank screen, change only the LCD_* pins below to
  match the labels from your board schematic/product page.
*/

#define LCD_CS 15
#define LCD_DC 2
#define LCD_RST 0
#define LCD_BL 5

#define SENSOR_MAX_KPA 50.0f

RoundLcd roundLcd(LCD_DC, LCD_CS, LCD_RST, LCD_BL);
PressureDisplay pressureDisplay(roundLcd.display(), SENSOR_MAX_KPA);

static PressureReading mockPressureReading() {
  const float t = millis() / 1000.0f;
  const uint32_t scenario = (millis() / 7000) % 6;
  float pressure = 22.0f + 9.0f * sin(t * 0.75f) + 3.0f * sin(t * 2.1f);
  SensorStatus status = SensorStatus::OK;

  switch (scenario) {
  case 1:
    pressure = -1.2f;
    status = SensorStatus::UNDER_RANGE;
    break;
  case 2:
    pressure = SENSOR_MAX_KPA + 4.5f;
    status = SensorStatus::OVER_RANGE;
    break;
  case 3:
    pressure = 0.0f;
    status = SensorStatus::SENSOR_DISCONNECTED;
    break;
  case 4:
    pressure = SENSOR_MAX_KPA;
    status = SensorStatus::ADC_FAULT;
    break;
  case 5:
    pressure = constrain(pressure, 0.0f, SENSOR_MAX_KPA);
    status = SensorStatus::NOT_CALIBRATED;
    break;
  default:
    pressure = constrain(pressure, 0.0f, SENSOR_MAX_KPA);
    break;
  }

  return {pressure, status};
}

void setup() {
  Serial.begin(115200);
  delay(300);

  if (!roundLcd.begin()) {
    Serial.println("Display init failed. Check LCD pins and driver type.");
    while (true) {
      delay(1000);
    }
  }

  pressureDisplay.drawStaticPressureScreen();
  Serial.println("Display initialized.");
}

void loop() {
  static uint32_t lastUpdate = 0;

  if (millis() - lastUpdate >= 250) {
    lastUpdate = millis();
    const PressureReading reading = mockPressureReading();
    pressureDisplay.drawPressureData(reading);

    Serial.print("Pressure: ");
    Serial.print(reading.pressureKpa, 1);
    Serial.print(" kPa, status=");
    Serial.println(static_cast<int>(reading.status));
  }
}
