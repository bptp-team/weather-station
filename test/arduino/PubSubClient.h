#ifndef PUB_SUB_CLIENT_H
#define PUB_SUB_CLIENT_H

// Host stub of the PubSubClient library. It records what the firmware would
// have sent to the broker instead of opening a connection.

#include <cstdint>
#include <string>
#include <vector>

#include "WiFi.h"

class PubSubClient {
public:
  explicit PubSubClient(WiFiClient &client);

  PubSubClient &setServer(const char *host, uint16_t port);
  bool connect(const char *clientId);
  bool connected();
  bool loop();
  bool publish(const char *topic, const char *payload);
};

namespace fake {

// One message the firmware published.
struct MqttMessage {
  std::string topic;
  std::string payload;
};

void resetMqtt();

// Decides whether the next connect() attempts succeed. Defaults to true.
void setMqttConnectSucceeds(bool succeeds);

const std::vector<MqttMessage> &mqttMessages();
const std::vector<std::string> &mqttClientIds();
std::string mqttServerHost();
uint16_t mqttServerPort();
int mqttLoopCalls();

} // namespace fake

#endif
