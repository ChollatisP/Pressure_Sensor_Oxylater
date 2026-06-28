#include <Arduino.h>
#include <Arduino_GFX_Library.h>

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

static float mockPressureKpa() {
  const float t = millis() / 1000.0f;
  float pressure = 22.0f + 9.0f * sin(t * 0.75f) + 3.0f * sin(t * 2.1f);
  return constrain(pressure, 0.0f, SENSOR_MAX_KPA);
}

static void drawStaticPressureScreen() {
  display->fillScreen(COLOR_BG);

  display->fillCircle(120, 120, 114, COLOR_PANEL);
  display->drawCircle(120, 120, 115, COLOR_ACCENT);
  display->drawCircle(120, 120, 108, COLOR_GRID);

  display->setTextColor(COLOR_TEXT);
  display->setTextSize(2);
  display->setCursor(48, 28);
  display->print("MPX2050DP");

  display->setTextColor(COLOR_MUTED);
  display->setTextSize(1);
  display->setCursor(70, 50);
  display->print("mock pressure data");

  display->drawRoundRect(36, 76, 168, 62, 8, COLOR_GRID);
  display->drawRoundRect(46, 160, 148, 14, 7, COLOR_GRID);

  display->setTextColor(COLOR_MUTED);
  display->setCursor(53, 145);
  display->print("cmH2O");
  display->setCursor(153, 145);
  display->print("%FS");
}

static void drawPressureData(float pressureKpa) {
  const float cmH2O = pressureKpa * 10.1972f;
  const int percent = round((pressureKpa / SENSOR_MAX_KPA) * 100.0f);
  const int barWidth = map(percent, 0, 100, 0, 144);

  display->fillRect(48, 88, 144, 36, COLOR_PANEL);
  display->setTextColor(COLOR_ACCENT);
  display->setTextSize(4);
  display->setCursor(48, 88);
  display->print(pressureKpa, 1);

  display->setTextColor(COLOR_TEXT);
  display->setTextSize(1);
  display->setCursor(157, 112);
  display->print("kPa");

  display->fillRect(45, 160, 150, 16, COLOR_PANEL);
  display->fillRoundRect(48, 163, barWidth, 8, 4, COLOR_AMBER);

  display->fillRect(38, 185, 166, 18, COLOR_PANEL);
  display->setTextColor(COLOR_TEXT);
  display->setTextSize(2);
  display->setCursor(39, 186);
  display->print(cmH2O, 0);

  display->setCursor(150, 186);
  display->print(percent);
  display->print("%");
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

  drawStaticPressureScreen();
  Serial.println("Display initialized.");
}

void loop() {
  static uint32_t lastUpdate = 0;

  if (millis() - lastUpdate >= 250) {
    lastUpdate = millis();
    const float pressureKpa = mockPressureKpa();
    drawPressureData(pressureKpa);

    Serial.print("Pressure: ");
    Serial.print(pressureKpa, 1);
    Serial.println(" kPa");
  }
}
