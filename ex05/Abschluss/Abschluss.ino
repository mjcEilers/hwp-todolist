#include "config.h"
#include "Gyro.h"
#include <LiquidCrystal.h>

Gyro gyro(GYRO_PIN);
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

void setup() {
  gyro.begin();
  lcd.begin(NUM_CHARS, NUM_LINES);
  lcd.setCursor(0, 0);
  lcd.print("Heading: ");
}

void loop() {
  gyro.update();

  float angle = gyro.getAngle();
  lcd.setCursor(9, 0);
  lcd.print(angle);
  delay(100);
}
