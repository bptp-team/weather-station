#include "Logger.h"

#include <Arduino.h>

const char *getLogLevelName(LogLevel level) {
  switch (level) {
  case INFO:
    return "INFO";
  case WARNING:
    return "WARNING";
  case ERROR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}

void logEvent(LogLevel level, const char *sourceName, const char *message) {
  const char *levelName = getLogLevelName(level);

  Serial.printf("[%lu] [%s] [%s] %s\n", millis(), levelName, sourceName,
                message);
}
