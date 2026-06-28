#ifndef PRESSURE_READING_H
#define PRESSURE_READING_H

enum class SensorStatus {
  OK,
  UNDER_RANGE,
  OVER_RANGE,
  ADC_FAULT,
  SENSOR_DISCONNECTED,
  NOT_CALIBRATED
};

struct PressureReading {
  float pressureKpa;
  SensorStatus status;
};

#endif
