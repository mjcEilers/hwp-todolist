#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

class Ultrasonic {
public:
  Ultrasonic(uint8_t pin);
  float getDistanceCM();

private:
  uint8_t _pin;
  static const unsigned long TIMEOUT = 30000UL; // 30ms
};

#endif
