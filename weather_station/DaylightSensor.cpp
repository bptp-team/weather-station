#include "DaylightSensor.h"

#include <Arduino.h>

namespace {
const char *DAYLIGHT_STATE_DAY = "DAY";
const char *DAYLIGHT_STATE_NIGHT = "NIGHT";
}

DaylightSensor::DaylightSensor(int digitalSignalPin)
    : digitalSignalPin(digitalSignalPin) {}

void DaylightSensor::begin() { pinMode(digitalSignalPin, INPUT); }

const char *DaylightSensor::readDaylightState() const {
  // The LDR module pulls its digital output LOW once light is detected.
  bool isLightDetected = digitalRead(digitalSignalPin) == LOW;

  return isLightDetected ? DAYLIGHT_STATE_DAY : DAYLIGHT_STATE_NIGHT;
}
