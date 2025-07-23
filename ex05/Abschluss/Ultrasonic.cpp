#include "Ultrasonic.h"

const unsigned long TIMEOUT = 30;

Ultrasonic::Ultrasonic(uint8_t pin) : _pin(pin) {}

void Ultrasonic::begin() {
  for (uint8_t i = 0; i < 8; i++) {
    this->update();
  }
}

void Ultrasonic::update() {
  // Send 10us HIGH pulse
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_pin, LOW);

  // Switch to input
  pinMode(_pin, INPUT);

  // Measure duration of echo pulse
  unsigned long duration = pulseIn(_pin, HIGH, TIMEOUT);

  float distance = (duration != 0) ? (duration * 0.0343) / 2.0 : -1.0;

  _buf.put(distance);
}

float Ultrasonic::getDistanceCM() {
  return _buf.getAvg();
}

void Ultrasonic::RingBufferF::put(float in) {
  if (in >= 0) {
    _buf[_currentI] = in;
    _currentI = (_currentI + 1) % _size;
  }
}

float Ultrasonic::RingBufferF::getAvg() {
  float sum = 0;
  for (uint8_t i = 0; i < _size; i++) {
    sum += _buf[i];
  }
  return sum / (float)_size;
}