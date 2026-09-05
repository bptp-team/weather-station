#ifndef MQTT_PUBLISHER_H
#define MQTT_PUBLISHER_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "WeatherReading.h"

class MqttPublisher {
 public:
  MqttPublisher(const char *wifiSsid, const char *wifiPassword,
                const char *mqttHost, uint16_t mqttPort,
                const char *deviceId);

  void begin();
  void maintainConnection();
  void publishReading(const WeatherReading &reading);

 private:
  static const unsigned long WIFI_RETRY_INTERVAL_MS = 10000;
  static const unsigned long MQTT_RETRY_INTERVAL_MS = 5000;

  WiFiClient wifiClient;
  PubSubClient mqttClient;

  const char *wifiSsid;
  const char *wifiPassword;
  const char *mqttHost;
  uint16_t mqttPort;
  
  const char *deviceId;

  bool wifiWasConnected = false;
  bool wifiFailureWasLogged = false;
  unsigned long lastWifiAttemptMs = 0;
  unsigned long lastMqttAttemptMs = 0;

  void connectToWifi();
  void connectToMqtt();

  String topicFor(const char *measurement) const;
  bool publishText(const char *measurement, const String &payload);
};

#endif
