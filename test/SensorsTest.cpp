#include "AirQualitySensor.h"
#include "Arduino.h"
#include "DaylightSensor.h"
#include "TestFramework.h"
#include "WaterLevelSensor.h"

namespace {

const int LDR_PIN = 34;
const int WATER_SIGNAL_PIN = 35;
const int WATER_POWER_PIN = 25;
const int WATER_SETTLE_MS = 10;
const int MQ135_PIN = 32;

std::string eventsSoFar() {
  return ::testing::join(fake::boardEvents(), ", ");
}

} // namespace

TEST(theDaylightSensorReadsOnlyItsOwnPin) {
  fake::resetBoard();
  fake::setAnalogValue(LDR_PIN, 3000);
  fake::setAnalogValue(MQ135_PIN, 7);

  DaylightSensor sensor(LDR_PIN);

  CHECK_EQ(sensor.readRawDaylight(), 3000);
}

TEST(theDaylightSensorWidensItsInputRangeOnBegin) {
  fake::resetBoard();

  DaylightSensor sensor(LDR_PIN);
  sensor.begin();

  CHECK_TEXT_EQ(eventsSoFar(), "analogSetPinAttenuation(34,ADC_11db)");
}

TEST(theAirQualitySensorReadsOnlyItsOwnPin) {
  fake::resetBoard();
  fake::setAnalogValue(MQ135_PIN, 1500);
  fake::setAnalogValue(LDR_PIN, 7);

  AirQualitySensor sensor(MQ135_PIN);

  CHECK_EQ(sensor.readRawAirQuality(), 1500);
}

TEST(theAirQualitySensorWidensItsInputRangeOnBegin) {
  fake::resetBoard();

  AirQualitySensor sensor(MQ135_PIN);
  sensor.begin();

  CHECK_TEXT_EQ(eventsSoFar(), "analogSetPinAttenuation(32,ADC_11db)");
}

TEST(theWaterProbeStartsUnpowered) {
  fake::resetBoard();

  WaterLevelSensor sensor(WATER_SIGNAL_PIN, WATER_POWER_PIN, WATER_SETTLE_MS);
  sensor.begin();

  CHECK_TEXT_EQ(eventsSoFar(),
                "pinMode(25,OUTPUT), digitalWrite(25,LOW), "
                "analogSetPinAttenuation(35,ADC_11db)");
  CHECK_EQ(fake::digitalValue(WATER_POWER_PIN), LOW);
}

TEST(theWaterProbeIsPoweredOnlyWhileItIsBeingRead) {
  fake::resetBoard();
  fake::setAnalogValue(WATER_SIGNAL_PIN, 1800);

  WaterLevelSensor sensor(WATER_SIGNAL_PIN, WATER_POWER_PIN, WATER_SETTLE_MS);
  sensor.begin();
  fake::clearBoardEvents();

  CHECK_EQ(sensor.readRawLevel(), 1800);

  // The probe must settle before the reading, and lose power right after it,
  // otherwise the electrodes corrode between measurements.
  CHECK_TEXT_EQ(eventsSoFar(), "digitalWrite(25,HIGH), delay(10), "
                               "analogRead(35), digitalWrite(25,LOW)");
  CHECK_EQ(fake::digitalValue(WATER_POWER_PIN), LOW);
}
