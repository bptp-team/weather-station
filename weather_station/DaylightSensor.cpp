#include "DaylightSensor.h"

#include <Arduino.h>

#include "Logger.h"

namespace {
const char *LOG_SOURCE = "LDR";
}

DaylightSensor::DaylightSensor(int analogSignalPin)
    : analogSignalPin(analogSignalPin) {}

void DaylightSensor::begin() {
  analogSetPinAttenuation(analogSignalPin, ADC_11db);

  logEvent(INFO, LOG_SOURCE, "Initialized");
}

int DaylightSensor::readRawDaylight() const {
  return analogRead(analogSignalPin);
}
