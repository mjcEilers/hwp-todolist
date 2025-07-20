#include "Motors.h"

Motors::Motors(uint8_t aPin1, uint8_t aPin2, uint8_t bPin1, uint8_t bPin2) {
  _motors[static_cast<uint8_t>(Motor::A)] = {aPin1, aPin2};
  _motors[static_cast<uint8_t>(Motor::B)] = {bPin1, bPin2};
}

void Motors::begin() {
  for (auto motor : _motors) {
    pinMode(motor.pin1, OUTPUT);
    pinMode(motor.pin2, OUTPUT);
    digitalWrite(motor.pin1, LOW);
    digitalWrite(motor.pin2, LOW);
  }
}

void Motors::setSpeed(uint8_t speed) {
  _speed = constrain(speed, 0, 255);
  setMotors(_currentDir);  // To apply speed change directly
}

void Motors::setMotors(Direction dir) {
  _currentDir = dir;
  switch (dir) {
    case STOP:
      stopMotor(Motor::A);
      stopMotor(Motor::B);
      break;

    case FORWARD:
      driveMotor(Motor::A, true);
      driveMotor(Motor::B, true);
      break;

    case BACKWARD:
      driveMotor(Motor::A, false);
      driveMotor(Motor::B, false);
      break;

    case LEFT:
      driveMotor(Motor::A, true);
      driveMotor(Motor::B, false);
      break;

    case RIGHT:
      driveMotor(Motor::A, false);
      driveMotor(Motor::B, true);
      break;
  }
}

void Motors::driveMotor(Motor m, bool forward) {
  uint8_t idx = static_cast<uint8_t>(m);
  uint16_t out = map(_speed, 0, 255, 0, 1023);

  if (forward) {
    analogWrite(_motors[idx].pin1, out);
    digitalWrite(_motors[idx].pin2, LOW);
  } else {
    digitalWrite(_motors[idx].pin1, LOW);
    analogWrite(_motors[idx].pin2, out);
  }
}

void Motors::stopMotor(Motor m) {
  uint8_t idx = static_cast<uint8_t>(m);
  digitalWrite(_motors[idx].pin1, LOW);
  digitalWrite(_motors[idx].pin2, LOW);
}