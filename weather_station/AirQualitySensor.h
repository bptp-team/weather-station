#ifndef AIR_QUALITY_SENSOR_H
#define AIR_QUALITY_SENSOR_H

// MQ135 gas sensor: reports how polluted the surrounding air is.
class AirQualitySensor {
public:
  explicit AirQualitySensor(int analogSignalPin);

  void begin();

  // Raw ADC value: higher means a higher concentration of detected gases.
  int readRawAirQuality() const;

private:
  int analogSignalPin;
};

#endif
