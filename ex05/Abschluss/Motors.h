#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

class Motors {
public:
  enum class Motor : uint8_t { A = 0, B = 1 };
  enum class Direction { STOP, FORWARD, BACKWARD, LEFT, RIGHT };

  Motors(uint8_t aPin1, uint8_t aPin2, uint8_t bPin1, uint8_t bPin2);
  void begin();
  void setMotors(Direction dir);
  void setSpeed(uint8_t speed);

private:
  struct MotorPins {
    uint8_t pin1;
    uint8_t pin2;
  } _motors[2];

  uint8_t _speed;
  Direction _currentDir = Direction::STOP;

  void driveMotor(Motor m, bool forward);
  void stopMotor(Motor m);
};

#endif