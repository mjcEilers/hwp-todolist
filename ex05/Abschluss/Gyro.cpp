#include "Gyro.h"
#include <math.h>

Gyro::Gyro(uint8_t pin, Mode mode) : _pin(pin), _mode(mode) {
  float scale = (_mode == Mode::X) ? 0.0025 : 0.0100;
  _factor = (5.0 / 1023.0) / scale;
};

void Gyro::begin() {
  pinMode(_pin, INPUT);
  calibrate();
}

void Gyro::calibrate() {
  const uint8_t sampleSize = 100;

  uint32_t sum = 0;
  for (uint8_t i = 0; i < sampleSize; i++) {
    sum += analogRead(_pin);
    delay(2);
  }

  _zeroOffset = sum / sampleSize;
  _heading = 0;
}

void Gyro::update() {
  unsigned long now = millis();
  unsigned long dt = (now - _lastUpdateTime) / 1000.0;
  _lastUpdateTime = now;

  int16_t turn_rate = analogRead(_pin) - _zeroOffset;
  _heading += turn_rate * dt;
}

float Gyro::getAngle() {
  return fmod((360.0 * _heading) * _factor, 360.);
}