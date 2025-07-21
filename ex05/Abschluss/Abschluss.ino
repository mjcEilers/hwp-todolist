#include "config.h"
#include "Gyro.h"
#include <LiquidCrystal.h>

#define GYRO_FULL_TURN 1140091

Gyro gyro(GYRO_PIN, 30);
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

uint16_t no_turn;
void setup() {
  Serial.begin(COMSPEED);
  gyro.begin();
  lcd.begin(NUM_CHARS, NUM_LINES);
  lcd.setCursor(0, 0);
  lcd.print("Heading: ");
  lcd.setCursor(0, 1);
  lcd.print("Angle: ");
  // lcd.setCursor(0, 2);
  // // lcd.print("heading:");
  // lcd.print("target: xxxdeg");
  // lcd.setCursor(0, 3);
  // lcd.print("heading: xxxdeg");
}

void loop() {
  gyro.update();

  int32_t heading = gyro.getHeading();
  lcd.setCursor(9, 0);
  lcd.print(heading);

  float angle = gyro.getAngle();
  lcd.setCursor(7, 1);
  lcd.print(angle);
  // delay(100);

  // Aufgabe 1
  // int16_t adcValue;
  // // read the current ADC value on A3
  // adcValue = analogRead(GYRO_PIN);
  // lcd.setCursor(10, 0);
  // lcd.print("         ");
  // lcd.setCursor(10, 0);
  // lcd.print(adcValue);

  // // Aufgabe 2
  // int16_t turn_rate;
  // // calculate turn-rate
  // turn_rate = adcValue - no_turn;
  // lcd.setCursor(11, 1);
  // lcd.print("        ");
  // lcd.setCursor(11, 1);
  // lcd.print(turn_rate);
// 
  // 
  // static unsigned long last_time = 0;
  // static int32_t heading_int = 0;
  // // calculate direction
  // unsigned long current_time = millis();
  // unsigned long interval = current_time - last_time;
  // last_time = current_time;
  // // deadband for noise
  // if (abs(turn_rate) > 40){
  //   heading_int += turn_rate * interval;
  // }

  // display heading_int in 3rd line of lcd
  // lcd.setCursor(9, 2);
  // lcd.print("          ");
  // lcd.setCursor(9, 2);
  // lcd.print(heading_int);

  // Aufgabe 4
  // heading in degrees
  // int16_t heading = ((heading_int * 360)/ GYRO_FULL_TURN) % 360;
  // lcd.setCursor(9, 3);
  // lcd.print("   ");
  // lcd.setCursor(9, 3);
  // lcd.print(heading);

  delay(20);
}
