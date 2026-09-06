#include "PubSubClient.h"

namespace {

bool connectSucceeds = true;
bool isConnected = false;
int loopCalls = 0;
std::string serverHost;
uint16_t serverPort = 0;
std::vector<fake::MqttMessage> messages;
std::vector<std::string> clientIds;

} // namespace

PubSubClient::PubSubClient(WiFiClient &) {}

PubSubClient &PubSubClient::setServer(const char *host, uint16_t port) {
  serverHost = host == nullptr ? "" : host;
  serverPort = port;
  return *this;
}

bool PubSubClient::connect(const char *clientId) {
  clientIds.push_back(clientId == nullptr ? "" : clientId);
  isConnected = connectSucceeds;
  return isConnected;
}

bool PubSubClient::connected() { return isConnected; }

bool PubSubClient::loop() {
  loopCalls++;
  return isConnected;
}

bool PubSubClient::publish(const char *topic, const char *payload) {
  if (!isConnected) {
    return false;
  }

  messages.push_back({topic == nullptr ? "" : topic,
                      payload == nullptr ? "" : payload});
  return true;
}

namespace fake {

void resetMqtt() {
  connectSucceeds = true;
  isConnected = false;
  loopCalls = 0;
  serverHost.clear();
  serverPort = 0;
  messages.clear();
  clientIds.clear();
}

void setMqttConnectSucceeds(bool succeeds) { connectSucceeds = succeeds; }

const std::vector<MqttMessage> &mqttMessages() { return messages; }

const std::vector<std::string> &mqttClientIds() { return clientIds; }

std::string mqttServerHost() { return serverHost; }

uint16_t mqttServerPort() { return serverPort; }

int mqttLoopCalls() { return loopCalls; }

} // namespace fake
