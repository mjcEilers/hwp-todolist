#include "Ultrasonic.h"

const unsigned long TIMEOUT = 30;

Ultrasonic::Ultrasonic(uint8_t pin) : _pin(pin) {}

float Ultrasonic::getDistanceCM() {
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

  if (duration == 0) {
    return -1.0;
  }

  return (duration * 0.0343) / 2.0;
}