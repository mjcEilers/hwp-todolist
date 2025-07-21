#include "config.h"
#include "Gyro.h"
#include "Motors.h"
#include "ButtonArray.h"
#include "Ultrasonic.h"
#include <LiquidCrystal.h>

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

static State states[] = { IDLE, FORWARD, RIGHTTURN, FORWARD, LEFTTURN, FORWARD, LEFTTURN, IDLE };
static float headings[] = { 0.0, 0.0, -90.0, -90.0, 0.0, 0.0, 90.0, 90.0};
uint8_t stateIndex = 0;

const ButtonArray::Button startStopButton = ButtonArray::Button::S3;
const float d = 60.0;

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

  //getDirection();
  //motors.setMotors(Motors::Direction::FORWARD);

  switch (states[state_index]) {
    case IDLE:
      motors.stopMotors();
      if (buttons.isPressed(startStopButton)) {
        state_index++;
        gyro.calibrate();
      }
      break;

    case FORWARD:
      motors.setMotors(Motors::Direction::FORWARD);
      if (distance > 0 && distance < d) {
        state_index++;
      }
      break;

    case LEFTTURN:
      motors.setMotors(Motors::Direction::LEFT);
      if ()

    case RIGHTTURN:
      break;
  }

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
