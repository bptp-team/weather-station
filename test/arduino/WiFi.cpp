#include "WiFi.h"

WiFiStub WiFi;

namespace {

wl_status_t status = WL_DISCONNECTED;
std::vector<fake::WifiConnectAttempt> attempts;

} // namespace

wl_status_t WiFiStub::status() const { return ::status; }

void WiFiStub::mode(wifi_mode_t) {}

void WiFiStub::begin(const char *ssid, const char *password) {
  attempts.push_back({ssid == nullptr ? "" : ssid,
                      password == nullptr ? "" : password});
}

namespace fake {

void resetWifi() {
  ::status = WL_DISCONNECTED;
  attempts.clear();
}

void setWifiStatus(wl_status_t newStatus) { ::status = newStatus; }

const std::vector<WifiConnectAttempt> &wifiConnectAttempts() {
  return attempts;
}

} // namespace fake
