#ifndef WATER_LEVEL_SENSOR_H
#define WATER_LEVEL_SENSOR_H

// Resistive water level probe. It is powered only while being read, so the
// electrodes do not corrode between measurements.
class WaterLevelSensor {
public:
  WaterLevelSensor(int analogSignalPin, int powerPin, int settleDelayMs);

  void begin();

  // Raw ADC value: higher means more of the probe is submerged.
  int readRawLevel();

private:
  void powerOn();
  void powerOff();

  int analogSignalPin;
  int powerPin;
  int settleDelayMs;
};

#endif
