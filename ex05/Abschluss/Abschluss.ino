#include "config.h"
#include "Gyro.h"
#include "Motors.h"
#include "ButtonArray.h"
#include "Ultrasonic.h"
#include <LiquidCrystal.h>
#include <math.h>

#define GYRO_FULL_TURN 1140091

Ultrasonic us2(US2_PIN);
Gyro gyro(GYRO_PIN, 30);
Motors motors(MOTOR_A1_PIN, MOTOR_A2_PIN, MOTOR_B1_PIN, MOTOR_B2_PIN);
ButtonArray buttons(BUTTONS_PIN, 50, 30);
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

enum State {
  IDLE,
  FORWARD,
  RIGHTTURN,
  LEFTTURN
};

static State states[9] = { IDLE, FORWARD, RIGHTTURN, FORWARD, LEFTTURN, FORWARD, LEFTTURN, FORWARD, IDLE };
static float headings[9] = { 0.0, 0.0, -90.0, -90.0, 0.0, 0.0, 90.0, 90.0, 90.0};
uint8_t stateIndex = 0;

const ButtonArray::Button startStopButton = ButtonArray::Button::S3;
const float d = 30.0;

/// returns true if INPUT is in [TARGET - EPSILON, TARGET + EPSILON] and else false
bool isDegClose(float input, float target, float epsilon) {
  float diff = fmod(abs(input - target), 360.0);
  if(diff > 180.) {
    diff = 360. - diff;
  }
  return diff <= epsilon;
}

void setup() {
  Serial.begin(COMSPEED);
  gyro.begin();
  motors.begin();
  lcd.begin(NUM_CHARS, NUM_LINES);
  lcd.setCursor(0, 0);
  lcd.print("Heading: ");
  lcd.setCursor(0, 1);
  lcd.print("Angle: ");
  lcd.setCursor(0, 2);
  lcd.print("Distance: ");
  motors.setSpeed(50);
}

void loop() {
  gyro.update();
  buttons.update();

  int32_t heading = gyro.getHeading();
  lcd.setCursor(9, 0);
  lcd.print(heading);

  float angle = gyro.getAngle();
  lcd.setCursor(7, 1);
  lcd.print(angle);

  float distance = us2.getDistanceCM();
  lcd.setCursor(10, 2);
  lcd.print(distance);

  // motors.setMotors(Motors::Direction::FORWARD);
  setSpeed();

  switch (states[stateIndex]) {
    case IDLE:
      motors.setMotors(Motors::Direction::STOP);
      if (buttons.isPressed(startStopButton)) {
        stateIndex++;
        gyro.calibrate();
      }
      break;

    case FORWARD:
      motors.setMotors(Motors::Direction::FORWARD);
      if (distance > 0 && distance < d) {
        stateIndex++;
      }
      break;

    case LEFTTURN:
      motors.setMotors(Motors::Direction::LEFT);
      if (isDegClose(angle, headings[stateIndex], 2)) {
        stateIndex++;
      }
      break;

    case RIGHTTURN:
      motors.setMotors(Motors::Direction::RIGHT);
      if (isDegClose(angle, headings[stateIndex], 2)) {
        stateIndex++;
      }
      break;
  }

  delay(20);
}

void setSpeed() {
  uint8_t speed;
  switch (states[stateIndex]) {
    case FORWARD:
      speed = 20;
    default:
      speed = 15;
  }

  motors.setSpeed(speed);
}
