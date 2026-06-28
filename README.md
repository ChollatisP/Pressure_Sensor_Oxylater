# Pressure Sensor Oxylater

Pressure Sensor Oxylater is an ESP8266-based firmware project for displaying
pressure data on an ESP Round LCD V1.0 module. The current firmware runs on an
ESP-12S / NodeMCU-style target and shows a live mockup of MPX2050DP pressure
data on the built-in round LCD.

## Project Status

The display firmware is currently set up as a visual prototype. It generates
simulated MPX2050DP pressure readings in software, then renders the values on
the LCD as:

- pressure in `kPa`
- converted pressure in `cmH2O`
- percent of full scale
- animated bar graph
- serial debug output

This lets the display UI be tested before the real pressure sensor circuit is
connected and calibrated.

## Hardware Target

- MCU: ESP-12S / ESP8266
- Display: ESP Round LCD V1.0 with 240 x 240 round TFT
- Assumed display controller: GC9A01
- Sensor planned: MPX2050DP differential pressure sensor

The firmware uses ESP8266 hardware SPI for the LCD. Hardware SPI pins are fixed
on ESP8266:

| Signal | ESP8266 GPIO |
| --- | --- |
| SCK | GPIO14 |
| MOSI | GPIO13 |
| MISO | GPIO12, not used by the LCD |

Current display control pins in `main.cpp`:

| LCD Signal | ESP8266 GPIO |
| --- | --- |
| CS | GPIO15 |
| DC | GPIO2 |
| RST | GPIO0 |
| BL | GPIO5 |

If the screen is blank after upload, check the board schematic or silkscreen and
update these pin definitions in `Firmware/PressureSensor/src/main.cpp`.

## Firmware

The PlatformIO firmware project is located at:

```text
Firmware/PressureSensor
```

Main application file:

```text
Firmware/PressureSensor/src/main.cpp
```

PlatformIO environment:

```ini
[env:nodemcuv2]
platform = espressif8266
board = nodemcuv2
framework = arduino
```

Main display dependency:

```ini
moononournation/GFX Library for Arduino
```

## MPX2050DP Sensor Note

The MPX2050DP is a 50 kPa differential pressure sensor, but its raw output is a
small millivolt-level signal. It normally needs an instrumentation amplifier or
other signal-conditioning circuit before it can be read reliably by the ESP8266
ADC.

The current firmware uses this mock function instead of reading the real sensor:

```cpp
static float mockPressureKpa();
```

When the analog front-end is ready, this function can be replaced with calibrated
ADC reading code.

## Repository Layout

```text
CAD_File/                 Mechanical CAD files
Firmware/PressureSensor/  PlatformIO firmware project
README.md                 Project overview
```

## Build And Upload

Open `Firmware/PressureSensor` in PlatformIO, then build and upload to the
ESP-12S / ESP8266 target.

The serial monitor runs at:

```text
115200 baud
```
