#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "PressureDisplay.h"

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

#define LCD_WIDTH 240
#define LCD_HEIGHT 240
#define SENSOR_MAX_KPA 50.0f

Arduino_DataBus *bus = new Arduino_HWSPI(
    LCD_DC,
    LCD_CS);

Arduino_GFX *display = new Arduino_GC9A01(
    bus,
    LCD_RST,
    0,
    true,
    LCD_WIDTH,
    LCD_HEIGHT);

PressureDisplay pressureDisplay(display, SENSOR_MAX_KPA);

static float mockPressureKpa() {
  const float t = millis() / 1000.0f;
  float pressure = 22.0f + 9.0f * sin(t * 0.75f) + 3.0f * sin(t * 2.1f);
  return constrain(pressure, 0.0f, SENSOR_MAX_KPA);
}

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);

  if (!display->begin()) {
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
    const float pressureKpa = mockPressureKpa();
    pressureDisplay.drawPressureData(pressureKpa);

    Serial.print("Pressure: ");
    Serial.print(pressureKpa, 1);
    Serial.println(" kPa");
  }
}
