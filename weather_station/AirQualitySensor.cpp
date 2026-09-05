#include "AirQualitySensor.h"

#include <Arduino.h>

#include "Logger.h"

namespace {
const char *LOG_SOURCE = "MQ135";
}

AirQualitySensor::AirQualitySensor(int analogSignalPin)
    : analogSignalPin(analogSignalPin) {}

void AirQualitySensor::begin() {
  analogSetPinAttenuation(analogSignalPin, ADC_11db);

  logEvent(INFO, LOG_SOURCE, "Initialized");
}

int AirQualitySensor::readRawAirQuality() const {
  return analogRead(analogSignalPin);
}
