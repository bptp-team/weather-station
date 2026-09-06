#include "Logger.h"

#include "Arduino.h"
#include "TestFramework.h"

TEST(everySeverityHasAName) {
  CHECK_TEXT_EQ(getLogLevelName(INFO), "INFO");
  CHECK_TEXT_EQ(getLogLevelName(WARNING), "WARNING");
  CHECK_TEXT_EQ(getLogLevelName(ERROR), "ERROR");
}

TEST(anUnknownSeverityDoesNotBreakTheLogger) {
  CHECK_TEXT_EQ(getLogLevelName(static_cast<LogLevel>(42)), "UNKNOWN");
}

TEST(anEventCarriesTheUptimeSeverityAndSource) {
  fake::resetBoard();
  fake::setMillis(1234);

  logEvent(WARNING, "BME280", "Not found");

  CHECK_TEXT_EQ(fake::serialOutput(), "[1234] [WARNING] [BME280] Not found\n");
}

TEST(eachEventIsPrintedOnItsOwnLine) {
  fake::resetBoard();
  fake::setMillis(10);

  logEvent(INFO, "LDR", "Initialized");
  logEvent(INFO, "MQ135", "Initialized");

  CHECK_TEXT_EQ(fake::serialOutput(), "[10] [INFO] [LDR] Initialized\n"
                                      "[10] [INFO] [MQ135] Initialized\n");
}
