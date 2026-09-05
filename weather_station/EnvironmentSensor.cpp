#include "EnvironmentSensor.h"

#include "Logger.h"

namespace {
const char *LOG_SOURCE = "BME280";
}

EnvironmentSensor::EnvironmentSensor(uint8_t i2cAddress)
    : i2cAddress(i2cAddress) {}

void EnvironmentSensor::begin() {
  if (!bme280.begin(i2cAddress)) {
    logEvent(WARNING, LOG_SOURCE, "Not found");
    while (1)
      ;
  }

  logEvent(INFO, LOG_SOURCE, "Found");
}

float EnvironmentSensor::readTemperatureCelsius() {
  return bme280.readTemperature();
}

float EnvironmentSensor::readPressurePascals() { return bme280.readPressure(); }

float EnvironmentSensor::readRelativeHumidityPercent() {
  return bme280.readHumidity();
}
