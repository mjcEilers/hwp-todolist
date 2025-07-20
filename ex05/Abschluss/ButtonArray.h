#ifndef BUTTON_ARRAY_H
#define BUTTON_ARRAY_H

#include <Arduino.h>

class ButtonArray {
public:
  enum Button { NONE, S1, S2, S3, S4, S5 };

  ButtonArray(uint8_t inputPin, uint16_t threshold, uint16_t debounceDelay);
  void update();
  bool isPressed(Button button);
  Button getPressedButton();

private:
  uint8_t _pin;
  uint16_t _threshold;  // Noise threshold
  uint16_t _debounceDelay;

  Button _current;
  Button _lastStable;
  unsigned long _lastChangeTime;

  Button detectButton(uint16_t value);
};

#endif