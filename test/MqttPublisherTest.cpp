#include "MqttPublisher.h"

#include "Arduino.h"
#include "PubSubClient.h"
#include "TestFramework.h"
#include "WiFi.h"

namespace {

const char *WIFI_SSID = "your-wifi-network";
const char *WIFI_PASSWORD = "your-wifi-password";
const char *MQTT_HOST = "192.168.1.20";
const uint16_t MQTT_PORT = 1883;
const char *DEVICE_ID = "station-01";

// Far enough past boot that both the Wi-Fi and the MQTT retry windows are open.
const unsigned long AFTER_EVERY_RETRY_WINDOW_MS = 60000;

void resetFakes() {
  fake::resetBoard();
  fake::resetWifi();
  fake::resetMqtt();
}

MqttPublisher makePublisher() {
  return MqttPublisher(WIFI_SSID, WIFI_PASSWORD, MQTT_HOST, MQTT_PORT,
                       DEVICE_ID);
}

// Brings a publisher up to the point where the broker accepted the connection.
void connect(MqttPublisher &publisher) {
  fake::setWifiStatus(WL_CONNECTED);
  publisher.begin();
  fake::setMillis(AFTER_EVERY_RETRY_WINDOW_MS);
  publisher.maintainConnection();
}

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

TEST(beginTargetsTheConfiguredBrokerAndNetwork) {
  resetFakes();

  MqttPublisher publisher = makePublisher();
  publisher.begin();

  CHECK_TEXT_EQ(fake::mqttServerHost(), MQTT_HOST);
  CHECK_EQ(fake::mqttServerPort(), MQTT_PORT);
  CHECK_EQ(fake::wifiConnectAttempts().size(), size_t(1));
  CHECK_TEXT_EQ(fake::wifiConnectAttempts()[0].ssid, WIFI_SSID);
  CHECK_TEXT_EQ(fake::wifiConnectAttempts()[0].password, WIFI_PASSWORD);
}

TEST(theBrokerIsNotContactedBeforeWifiIsUp) {
  resetFakes();
  fake::setWifiStatus(WL_DISCONNECTED);

  MqttPublisher publisher = makePublisher();
  publisher.begin();
  fake::setMillis(AFTER_EVERY_RETRY_WINDOW_MS);
  publisher.maintainConnection();

  CHECK_TRUE(fake::mqttClientIds().empty());
}

TEST(wifiIsRetriedOnlyOnceTheRetryWindowElapsed) {
  resetFakes();
  fake::setWifiStatus(WL_DISCONNECTED);

  MqttPublisher publisher = makePublisher();
  publisher.begin();

  fake::setMillis(9999);
  publisher.maintainConnection();
  CHECK_EQ(fake::wifiConnectAttempts().size(), size_t(1));

  fake::setMillis(10000);
  publisher.maintainConnection();
  CHECK_EQ(fake::wifiConnectAttempts().size(), size_t(2));
}

TEST(theClientIdIdentifiesTheDeviceAndTheBoard) {
  resetFakes();

  MqttPublisher publisher = makePublisher();
  connect(publisher);

  CHECK_EQ(fake::mqttClientIds().size(), size_t(1));
  CHECK_TEXT_EQ(fake::mqttClientIds()[0], "station-01-44556677");
}

TEST(anEstablishedConnectionIsServicedInsteadOfReopened) {
  resetFakes();

  MqttPublisher publisher = makePublisher();
  connect(publisher);
  publisher.maintainConnection();

  CHECK_EQ(fake::mqttClientIds().size(), size_t(1));
  CHECK_EQ(fake::mqttLoopCalls(), 1);
}

TEST(aReadingIsPublishedOnTheDocumentedTopics) {
  resetFakes();

  MqttPublisher publisher = makePublisher();
  connect(publisher);
  publisher.publishReading(sampleReading());

  const std::vector<fake::MqttMessage> &messages = fake::mqttMessages();
  CHECK_EQ(messages.size(), size_t(6));

  CHECK_TEXT_EQ(messages[0].topic, "weather/station-01/airTemperature");
  CHECK_TEXT_EQ(messages[0].payload, "21.50");
  CHECK_TEXT_EQ(messages[1].topic, "weather/station-01/airPressure");
  CHECK_TEXT_EQ(messages[1].payload, "101325.00");
  CHECK_TEXT_EQ(messages[2].topic, "weather/station-01/airHumidity");
  CHECK_TEXT_EQ(messages[2].payload, "48.25");
  CHECK_TEXT_EQ(messages[3].topic, "weather/station-01/daylight");
  CHECK_TEXT_EQ(messages[3].payload, "2048");
  CHECK_TEXT_EQ(messages[4].topic, "weather/station-01/waterLevel");
  CHECK_TEXT_EQ(messages[4].payload, "1024");
  CHECK_TEXT_EQ(messages[5].topic, "weather/station-01/airQuality");
  CHECK_TEXT_EQ(messages[5].payload, "512");
}

TEST(readingsAreDroppedWhileTheBrokerIsUnreachable) {
  resetFakes();
  fake::setMqttConnectSucceeds(false);

  MqttPublisher publisher = makePublisher();
  connect(publisher);
  publisher.publishReading(sampleReading());

  CHECK_TRUE(fake::mqttMessages().empty());
}
