#ifndef DAYLIGHT_SENSOR_H
#define DAYLIGHT_SENSOR_H

// LDR module: tells whether the station is under daylight or in the dark.
class DaylightSensor {
public:
  explicit DaylightSensor(int digitalSignalPin);

  void begin();

  // Returns "DAY" while there is light on the sensor, "NIGHT" otherwise.
  const char *readDaylightState() const;

private:
  int digitalSignalPin;
};

#endif
