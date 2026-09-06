#ifndef WEATHER_STATION_SECRETS_EXAMPLE_H
#define WEATHER_STATION_SECRETS_EXAMPLE_H

#define WIFI_SSID "your-wifi-network"
#define WIFI_PASSWORD "your-wifi-password"

// Use the Docker host's LAN IP, not localhost.
#define MQTT_HOST "192.168.1.20"
#define MQTT_PORT 1883
#define DEVICE_ID "station-01"

// Optional: how often the station reads its sensors, in
// milliseconds. Remove this line to keep the 10000 ms default.
#define READING_INTERVAL_MS 10000

#endif
