#include "WeatherReadingLogger.h"

#include <Arduino.h>

void logWeatherReading(LogLevel level, const WeatherReading &reading) {
  const char *levelName = getLogLevelName(level);

  Serial.printf("[%lu] [%s] temperature=%.2f pressure=%.2f humidity=%.2f "
                "light=%d water=%d air=%d\n",
                millis(), levelName, reading.temperatureCelsius,
                reading.pressurePascals, reading.relativeHumidityPercent,
                reading.daylightRaw, reading.waterLevelRaw,
                reading.airQualityRaw);
}
