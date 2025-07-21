#include "config.h"
#include "Gyro.h"
#include "Motors.h"
#include "ButtonArray.h"
#include <LiquidCrystal.h>

#define GYRO_FULL_TURN 1140091

Gyro gyro(GYRO_PIN, 30);
Motors motors(MOTOR_A1_PIN, MOTOR_A2_PIN, MOTOR_B1_PIN, MOTOR_B2_PIN);
ButtonArray buttons(BUTTONS_PIN, 50, 30);
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

void setup() {
  Serial.begin(COMSPEED);
  gyro.begin();
  motors.begin();
  lcd.begin(NUM_CHARS, NUM_LINES);
  lcd.setCursor(0, 0);
  lcd.print("Heading: ");
  lcd.setCursor(0, 1);
  lcd.print("Angle: ");
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

  getDirection();
  //motors.setMotors(Motors::Direction::FORWARD);

  delay(20);
}

void getDirection() {
  switch (buttons.getPressedButton()) {
    case ButtonArray::Button::S1:
      motors.setMotors(Motors::Direction::FORWARD);
      break;

    case ButtonArray::Button::S2:
      motors.setMotors(Motors::Direction::RIGHT);
      break;

    case ButtonArray::Button::S3:
      motors.setMotors(Motors::Direction::STOP);
      break;
    
    case ButtonArray::Button::S4:
      motors.setMotors(Motors::Direction::LEFT);
      break;

    case ButtonArray::Button::S5:
      motors.setMotors(Motors::Direction::BACKWARD);
      break;
  }
}
