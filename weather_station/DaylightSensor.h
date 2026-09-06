#ifndef DAYLIGHT_SENSOR_H
#define DAYLIGHT_SENSOR_H

// LDR module: reports how much light is falling on the station.
class DaylightSensor {
public:
  explicit DaylightSensor(int analogSignalPin);

  void begin();

  // Raw ADC value: higher means more light reaching the sensor.
  int readRawDaylight() const;

private:
  int analogSignalPin;
};

#endif
