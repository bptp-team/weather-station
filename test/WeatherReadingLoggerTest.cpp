#include "WeatherReadingLogger.h"

#include "Arduino.h"
#include "TestFramework.h"

namespace {

// Values chosen to be exactly representable as floats, so the expected text
// does not depend on rounding.
WeatherReading sampleReading() {
  WeatherReading reading;

  reading.temperatureCelsius = 21.5f;
  reading.pressurePascals = 101325.0f;
  reading.relativeHumidityPercent = 48.25f;
  reading.daylightRaw = 2048;
  reading.waterLevelRaw = 1024;
  reading.airQualityRaw = 512;

  return reading;
}

} // namespace

TEST(aReadingIsLoggedAsASingleLineOfKeyValuePairs) {
  fake::resetBoard();
  fake::setMillis(5000);

  logWeatherReading(INFO, sampleReading());

  CHECK_TEXT_EQ(fake::serialOutput(),
                "[5000] [INFO] temperature=21.50 pressure=101325.00 "
                "humidity=48.25 light=2048 water=1024 air=512\n");
}
