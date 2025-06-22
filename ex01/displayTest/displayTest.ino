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

// Trimmer Defines
#define TRIMMER A0

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

int getButtonI(int inp) {
  // Button 1: 0
  // Button 2: 242
  // Button 3: 473
  // Button 4: 677
  // Button 5: 830
  if (inp < 121) {
    return 1;
  }else if(inp < 357) {
    return 2;
  }else if(inp < 575) {
    return 3;
  }else if(inp < 753) {
    return 4;
  }else if(inp < 926) {
    return 5;
  }else {
    return 0;
  }
}

const char* getButtonC(int inp) {
  // Button 1: 0
  // Button 2: 242
  // Button 3: 473
  // Button 4: 677
  // Button 5: 830
  if (inp < 121) {
    return "S1";
  }else if(inp < 357) {
    return "S2";
  }else if(inp < 575) {
    return "S3";
  }else if(inp < 753) {
    return "S4";
  }else if(inp < 926) {
    return "S5";
  }else {
    return "- ";
  }
}

float get_frequency() {
  int raw = analogRead(TRIMMER);
  return map(raw, 0, MAX_READ, 1., 50.);
}

void loop() {
  // Set cursor to arbitrary position
  lcd.setCursor(10,0);

  // Read input
  int a0_read = analogRead(A0);
  float voltage = a0_read * REFERENCE_VOLTAGE / MAX_READ;
  char digits[4];
  dtostrf(voltage, 4, 2, digits);
  // Print row 0
  lcd.print(digits);

  lcd.setCursor(8,1);
  int a1_read = analogRead(A1);
  // Print row 1
  lcd.print((char[2])getButtonC(a1_read));
  // Prevent display flickering for too fast updates
  delay(100);
  // NOTE: If you update only parts of the screen, don't use lcd.clear.
  // Set the cursor to the line and column to be updated and override existing chars.
}
