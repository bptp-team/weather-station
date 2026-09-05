#ifndef WEATHER_READING_LOGGER_H
#define WEATHER_READING_LOGGER_H

#include "Logger.h"
#include "WeatherReading.h"

// Prints a full sensor snapshot as a single line of key=value pairs.
void logWeatherReading(LogLevel level, const WeatherReading &reading);

#endif
