#include "WaterLevelSensor.h"

#include <Arduino.h>

#include "Logger.h"

namespace {
const char *LOG_SOURCE = "WATER";
}

WaterLevelSensor::WaterLevelSensor(int analogSignalPin, int powerPin,
                                   int settleDelayMs)
    : analogSignalPin(analogSignalPin), powerPin(powerPin),
      settleDelayMs(settleDelayMs) {}

void WaterLevelSensor::begin() {
  pinMode(powerPin, OUTPUT);
  powerOff();

  analogSetPinAttenuation(analogSignalPin, ADC_11db);

  logEvent(INFO, LOG_SOURCE, "Initialized");
}

int WaterLevelSensor::readRawLevel() {
  powerOn();
  delay(settleDelayMs);

  int waterLevel = analogRead(analogSignalPin);

  powerOff();

  return waterLevel;
}

void WaterLevelSensor::powerOn() { digitalWrite(powerPin, HIGH); }

void WaterLevelSensor::powerOff() { digitalWrite(powerPin, LOW); }
