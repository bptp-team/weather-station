#ifndef LOGGER_H
#define LOGGER_H

// Severity of a log line, from the least to the most critical.
enum LogLevel { INFO, WARNING, ERROR };

// Prints a one-off event, e.g. a sensor that was initialized or not found.
// `sourceName` identifies who is reporting (e.g. "BME280").
void logEvent(LogLevel level, const char *sourceName, const char *message);

// Human readable name of a log level, used by every log formatter.
const char *getLogLevelName(LogLevel level);

#endif
