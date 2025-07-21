#include "config.h"
#include "Gyro.h"
#include <LiquidCrystal.h>

#define GYRO_FULL_TURN 1140091

Gyro gyro(GYRO_PIN, 30);
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

void setup() {
  Serial.begin(COMSPEED);
  gyro.begin();
  lcd.begin(NUM_CHARS, NUM_LINES);
  lcd.setCursor(0, 0);
  lcd.print("Heading: ");
  lcd.setCursor(0, 1);
  lcd.print("Angle: ");
}

void loop() {
  gyro.update();

  int32_t heading = gyro.getHeading();
  lcd.setCursor(9, 0);
  lcd.print(heading);

  float angle = gyro.getAngle();
  lcd.setCursor(7, 1);
  lcd.print(angle);

  delay(20);
}
