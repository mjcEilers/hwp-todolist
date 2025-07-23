#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

class Ultrasonic {
public:
  Ultrasonic(uint8_t pin);
  void begin();
  void update();
  float getDistanceCM();

private:
  class RingBufferF {
  public:
    RingBufferF() {};
    void put(float in);
    float getAvg();
  private:
    static const uint8_t _size = 8;
    float _buf[_size];
    uint8_t _currentI = 0;
  } _buf;
  uint8_t _pin;
  static const unsigned long TIMEOUT = 30000UL; // 30ms
};

#endif
