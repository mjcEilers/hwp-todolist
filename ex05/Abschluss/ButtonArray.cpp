#include "ButtonArray.h"

// Constructor
ButtonArray::ButtonArray(uint8_t inputPin, uint16_t threshold, uint16_t debounceDelay)
  : _pin(inputPin), _threshold(threshold), _debounceDelay(debounceDelay),
    _current(NONE), _lastStable(NONE), _lastChangeTime(0) {}

void ButtonArray::update() {
  uint16_t analogValue = analogRead(_pin);
  Button reading = detectButton(analogValue);

  if (reading != _current) {
    _current = reading;
    _lastChangeTime = millis();
  }

  if ((millis() - _lastChangeTime) > _debounceDelay && reading != _lastStable) {
    _lastStable = reading;
  }
}

bool ButtonArray::isPressed(Button button) {
  return button == _lastStable;
}

ButtonArray::Button ButtonArray::getPressedButton() {
  return _lastStable;
}

ButtonArray::Button ButtonArray::detectButton(uint16_t value) {
  if (abs(value -   0) < _threshold) return S1;
  if (abs(value - 242) < _threshold) return S2;
  if (abs(value - 473) < _threshold) return S3;
  if (abs(value - 677) < _threshold) return S4;
  if (abs(value - 830) < _threshold) return S5;
  return NONE;
}