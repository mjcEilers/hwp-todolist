// Include LCD functions:
#include <LiquidCrystal.h>

// LCD Connection
#define R_S 8
#define E   9
#define DB4 10
#define DB5 11
#define DB6 12
#define DB7 13

// LCD Defines
#define NUM_CHAR 20
#define NUM_LINES 4

#define REFERENCE_VOLTAGE 3.3
#define MAX_READ 1023

// Global variables
// Define the LCD screen
LiquidCrystal lcd(R_S, E, DB4, DB5, DB6, DB7);

void setup() {
  // LCD has 4 lines with 20 chars
  lcd.begin(NUM_CHAR, NUM_LINES);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  lcd.print("Analog 0: x.xxV");
  lcd.setCursor(0,1);
  lcd.print("button: -");
}

void loop() {
  // Set cursor to arbitrary position
  lcd.setCursor(10, 0);

  // Read input
  int a0_read = analogRead(A0);
  float voltage = a0_read * REFERENCE_VOLTAGE / MAX_READ;
  char digits[4];
  dtostrf(voltage, 4, 2, digits);
  // Print statements
  lcd.print(digits);
  // Prevent display flickering for too fast updates
  lcd
  delay(100);
  // NOTE: If you update only parts of the screen, don't use lcd.clear.
  // Set the cursor to the line and column to be updated and override existing chars.
}
