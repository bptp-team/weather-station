#include "MqttPublisher.h"
#include "Logger.h"

const char *MQTT_LOG_SOURCE = "MQTT";

MqttPublisher::MqttPublisher(const char *wifiSsid, const char *wifiPassword,
                             const char *mqttHost, uint16_t mqttPort,
                             const char *deviceId)
    : mqttClient(wifiClient),
      wifiSsid(wifiSsid),
      wifiPassword(wifiPassword),
      mqttHost(mqttHost),
      mqttPort(mqttPort),
      deviceId(deviceId) {}

void MqttPublisher::begin() {
  mqttClient.setServer(mqttHost, mqttPort);
  connectToWifi();
}

void MqttPublisher::maintainConnection() {
  const unsigned long currentTimeMs = millis();
  const auto wifiStatus = WiFi.status();
  const bool isWifiConnected = wifiStatus == WL_CONNECTED;
  const bool hasWifiConnectionFailed = wifiStatus == WL_CONNECT_FAILED;
  const bool isWifiRetryDue =
      currentTimeMs - lastWifiAttemptMs >= WIFI_RETRY_INTERVAL_MS;

  if (isWifiConnected) {
    if (!wifiWasConnected) {
      logEvent(INFO, MQTT_LOG_SOURCE, "Connected to Wi-Fi");
      wifiWasConnected = true;
    }
    wifiFailureWasLogged = false;
  } else {
    if (wifiWasConnected) {
      logEvent(WARNING, MQTT_LOG_SOURCE, "Wi-Fi connection lost");
      wifiWasConnected = false;
    } else if (hasWifiConnectionFailed && !wifiFailureWasLogged) {
      logEvent(WARNING, MQTT_LOG_SOURCE, "Wi-Fi connection failed");
      wifiFailureWasLogged = true;
    }

    if (isWifiRetryDue) {
      connectToWifi();
    }
    return;
  }

  const bool isMqttConnected = mqttClient.connected();
  const bool isMqttRetryDue =
      currentTimeMs - lastMqttAttemptMs >= MQTT_RETRY_INTERVAL_MS;

  if (!isMqttConnected) {
    if (isMqttRetryDue) {
      connectToMqtt();
    }
    return;
  }

  mqttClient.loop();
}

void MqttPublisher::publishReading(const WeatherReading &reading) {
  if (!mqttClient.connected()) {
    return;
  }

  publishText("airTemperature", String(reading.temperatureCelsius, 2));
  publishText("airPressure", String(reading.pressurePascals, 2));
  publishText("airHumidity", String(reading.relativeHumidityPercent, 2));
  publishText("daylight", String(reading.daylightState));
  publishText("waterLevel", String(reading.waterLevelRaw));
  publishText("airQuality", String(reading.airQualityRaw));
}

void MqttPublisher::connectToWifi() {
  lastWifiAttemptMs = millis();
  wifiFailureWasLogged = false;
  logEvent(INFO, MQTT_LOG_SOURCE, "Connecting to Wi-Fi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSsid, wifiPassword);
}

void MqttPublisher::connectToMqtt() {
  lastMqttAttemptMs = millis();
  String clientId = String(deviceId) + "-" + String((uint32_t)ESP.getEfuseMac(), HEX);

  logEvent(INFO, MQTT_LOG_SOURCE, "Connecting to MQTT broker");
  if (mqttClient.connect(clientId.c_str())) {
    logEvent(INFO, MQTT_LOG_SOURCE, "Connected to MQTT broker");
  } else {
    logEvent(WARNING, MQTT_LOG_SOURCE, "MQTT connection failed");
  }
}

String MqttPublisher::topicFor(const char *measurement) const {
  return String("weather/") + deviceId + "/" + measurement;
}

bool MqttPublisher::publishText(const char *measurement, const String &payload) {
  String topic = topicFor(measurement);
  return mqttClient.publish(topic.c_str(), payload.c_str());
}
