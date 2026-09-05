#ifndef ENVIRONMENT_SENSOR_H
#define ENVIRONMENT_SENSOR_H

#include <Adafruit_BME280.h>
#include <Wire.h>

// BME280: reads temperature, barometric pressure and relative humidity.
class EnvironmentSensor {
public:
  explicit EnvironmentSensor(uint8_t i2cAddress);

  // Starts the sensor. Halts the board if it is not wired to the I2C bus,
  // since no weather reading makes sense without it.
  void begin();

  float readTemperatureCelsius();
  float readPressurePascals();
  float readRelativeHumidityPercent();

private:
  Adafruit_BME280 bme280;
  uint8_t i2cAddress;
};

#endif
