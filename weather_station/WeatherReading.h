#ifndef WEATHER_READING_H
#define WEATHER_READING_H

// A single snapshot of every sensor of the station, taken at the same moment.
struct WeatherReading {
  float temperatureCelsius;
  float pressurePascals;
  float relativeHumidityPercent;
  const char *daylightState;
  int waterLevelRaw;
  int airQualityRaw;
};

#endif
