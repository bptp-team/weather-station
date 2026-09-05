### Weather Station

<p align="justify">
    <img
        src="docs/espressif-systems.svg"
        width="50"
        height="50"
    />
    <img
        src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/cplusplus/cplusplus-original.svg"
        width="50"
        height="50"
    />
</p>

## Local development

This project targets a classic ESP32 board using the Arduino framework. The
ESP32 connects to Wi-Fi and publishes sensor readings to a Mosquitto broker on
the local network. Cloud services are not part of this setup.

### Arduino IDE setup

1. Install the ESP32 board package in Arduino IDE and select the board that
    matches the hardware.
2. Install these libraries through Library Manager:
    - Adafruit BME280 Library
    - Adafruit Unified Sensor
    - PubSubClient
3. Copy `weather_station/secrets.example.h` to
    `weather_station/secrets.h`.
4. Edit `weather_station/secrets.h` with the Wi-Fi credentials and the LAN
    address of the computer running Mosquitto.
5. Open `weather_station/weather_station.ino`, select the ESP32 board and
    port, then upload the sketch.

`secrets.h` is ignored by Git. Do not commit Wi-Fi passwords or other local
credentials.

Example local configuration:

```cpp
#define WIFI_SSID "your-wifi-network"
#define WIFI_PASSWORD "your-wifi-password"
#define MQTT_HOST "192.168.1.20"
#define MQTT_PORT 1883
#define DEVICE_ID "station-01"
```

`MQTT_HOST` must be the Docker host's LAN IP or a hostname resolvable by the
ESP32. Do not use `localhost`: from the ESP32, `localhost` means the ESP32
itself. `MQTT_PORT` must match the host port published by the Mosquitto Docker
Compose project.

### MQTT topics

Each station publishes under its stable device ID:

| Topic | Meaning | Payload |
| --- | --- | --- |
| `weather/<device-id>/airTemperature` | Air temperature | Celsius, decimal text |
| `weather/<device-id>/airPressure` | Atmospheric pressure | Pascals, decimal text |
| `weather/<device-id>/airHumidity` | Relative humidity | Percent, decimal text |
| `weather/<device-id>/daylight` | Daylight state | Text |
| `weather/<device-id>/waterLevel` | Raw water-level sensor value | Integer text |
| `weather/<device-id>/airQuality` | Raw air-quality sensor value | Integer text |

For example, station `station-01` publishes to
`weather/station-01/airTemperature`.

### Verify messages

Subscribe from a computer on the same network. Replace the host address and
port if your Mosquitto Compose configuration uses different values:

```bash
mosquitto_sub -h 127.0.0.1 -p 1883 -t "weather/#" -v
```

Use the Docker host's LAN IP instead of `127.0.0.1` when subscribing from a
different computer. The current local broker allows anonymous, unencrypted
connections and should remain restricted to a trusted development network.

The firmware services MQTT continuously and schedules sensor readings every
10 seconds, so temporary Wi-Fi or broker outages can recover without rebooting
the ESP32.
