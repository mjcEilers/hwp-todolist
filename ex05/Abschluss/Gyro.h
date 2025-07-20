#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>

class Gyro {
public:
  /// X4 means 4 times the precision
  enum class Mode { X, X4 };

  Gyro(uint8_t pin, Mode mode = Mode::X4);
  void begin();
  void update();

  /// Calibrate the gyro to set the 
  void calibrate();
  
  /// Get angle relative of the last calibration point
  float getAngle();

private:
  uint8_t _pin;
  Mode _mode;

  uint16_t _zeroOffset = 0;
  uint16_t _heading = 0;
  float _factor;
  unsigned long _lastUpdateTime = 0;
};

#endif