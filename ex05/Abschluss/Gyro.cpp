#include "Gyro.h"
#include <math.h>

Gyro::Gyro(uint8_t pin, uint8_t deadband, Mode mode) : _pin(pin), _mode(mode), _deadband(deadband) {
  float scale = (_mode == Mode::X) ? 0.0025 : 0.0100;
  _factor = (3.3 / 1023.0) / scale;

};

void Gyro::begin() {
  pinMode(_pin, INPUT);
  calibrate();
}

void Gyro::calibrate() {
  const uint8_t sampleSize = 100;

  delay(100);
  uint32_t sum = 0;
  for (uint8_t i = 0; i < sampleSize; i++) {
    sum += analogRead(_pin);
    delay(10);
  }

  _zeroOffset = sum / sampleSize;
  _heading = 0;
}

void Gyro::update() {
  uint32_t now = millis();
  uint32_t dt = now - _lastUpdateTime;
  _lastUpdateTime = now;

  int16_t turnRate = analogRead(_pin) - _zeroOffset;
  if (abs(turnRate) > _deadband) {
    _heading += (turnRate * (int16_t) dt) / 100;
  }
}

float Gyro::getAngle() {
  return fmod((0.1 * _heading) * _factor, 360.);
}