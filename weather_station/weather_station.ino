#include "AirQualitySensor.h"
#include "DaylightSensor.h"
#include "EnvironmentSensor.h"
#include "Logger.h"
#include "MqttPublisher.h"
#include "WaterLevelSensor.h"
#include "WeatherReading.h"
#include "WeatherReadingLogger.h"
#include "secrets.h"

const char *BOARD_LOG_SOURCE = "ESP32";

const unsigned long SERIAL_BAUD_RATE = 115200;
const int ANALOG_READ_RESOLUTION_BITS = 12;
const unsigned long READING_INTERVAL_MS = 10 * 1000;

const uint8_t BME280_I2C_ADDRESS = 0x76;
const int LDR_SIGNAL_PIN = 34;
const int WATER_SIGNAL_PIN = 35;
const int WATER_POWER_PIN = 25;
const int WATER_SETTLE_MS = 10;
const int MQ135_SIGNAL_PIN = 32;

EnvironmentSensor environmentSensor(BME280_I2C_ADDRESS);
DaylightSensor daylightSensor(LDR_SIGNAL_PIN);
WaterLevelSensor waterLevelSensor(WATER_SIGNAL_PIN, WATER_POWER_PIN,
                                  WATER_SETTLE_MS);
AirQualitySensor airQualitySensor(MQ135_SIGNAL_PIN);
MqttPublisher mqttPublisher(WIFI_SSID, WIFI_PASSWORD, MQTT_HOST, MQTT_PORT,
                            DEVICE_ID);

unsigned long lastReadingMs = 0;

WeatherReading readAllSensors() {
  WeatherReading reading;

  reading.temperatureCelsius = environmentSensor.readTemperatureCelsius();
  reading.pressurePascals = environmentSensor.readPressurePascals();
  reading.relativeHumidityPercent =
      environmentSensor.readRelativeHumidityPercent();
  reading.daylightRaw = daylightSensor.readRawDaylight();
  reading.waterLevelRaw = waterLevelSensor.readRawLevel();
  reading.airQualityRaw = airQualitySensor.readRawAirQuality();

  return reading;
}

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  logEvent(INFO, BOARD_LOG_SOURCE, "Initialized");

  analogReadResolution(ANALOG_READ_RESOLUTION_BITS);

  daylightSensor.begin();
  environmentSensor.begin();
  waterLevelSensor.begin();
  airQualitySensor.begin();
  mqttPublisher.begin();
}

void loop() {
  mqttPublisher.maintainConnection();

  const unsigned long currentTimeMs = millis();
  const bool isReadingDue =
      currentTimeMs - lastReadingMs >= READING_INTERVAL_MS;

  if (isReadingDue) {
    lastReadingMs = currentTimeMs;
    WeatherReading reading = readAllSensors();

    logWeatherReading(INFO, reading);
    mqttPublisher.publishReading(reading);
  }
}
