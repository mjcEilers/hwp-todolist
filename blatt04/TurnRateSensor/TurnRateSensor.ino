// TurnRate Sensor: read data from analog gyroscope, output heading on lcd

/* Connections:
 R/S : Pin 11
 R/W : not connected
 E   : Pin 12
 DB4 : Pin 13
 DB5 : Pin A0
 DB6 : Pin A1
 DB7 : Pin A2
 
 Motor:
 A1 : Pin 5
 A2 : Pin 6
 B1 : Pin 9
 B2 : Pin 10
 Turn-Rate Sensor: A3
*/
// include LCD functions:
#include <LiquidCrystal.h> 

// define the LCD screen
LiquidCrystal lcd(11, 12, 13, A0, A1, A2);

// LCD Defines
#define NUM_CHAR 20
#define NUM_LINES 4

// Pins of motor
#define PIN_MOTOR_A1 5
#define PIN_MOTOR_A2 6
#define PIN_MOTOR_B1 9
#define PIN_MOTOR_B2 10
#define SENSOR_A3 A3

enum Motor {
  A,
  B
};

struct MotorPins {
  uint8_t pin1;
  uint8_t pin2;
};

const MotorPins motorPins[2] = {
  {PIN_MOTOR_A1, PIN_MOTOR_A2},
  {PIN_MOTOR_B2, PIN_MOTOR_B1}
};

// Aufgabe 1
float getADC(){
  return analogRead(SENSOR_A3);
}

// Aufgabe 2
long no_turn = 0;
void calibrateTurnRate(){
  long add = 0;
  for (uint8_t i = 0; i < 100; i++){
    add += getADC();
  }
  no_turn = add / 100;
}

// Aufgabe 5
void setMotor(Motor motor, bool forward, uint16_t speed){
  uint16_t speed_10 = map(speed, 0, 65535, 0, 1023);  // map 16bit to 10bit value
  MotorPins pins = motorPins[motor];

  switch (forward){
    case true:
      // drive forward
      digitalWrite(pins.pin1, LOW);
      analogWrite(pins.pin2, speed_10);
      break;

    case false:
      // drive backward
      analogWrite(pins.pin1, speed_10);
      digitalWrite(pins.pin2, LOW);
      break;
  }
}

// initialization
void setup(){
  for (MotorPins pins : motorPins){
    pinMode(pins.pin1, OUTPUT);
    pinMode(pins.pin2, OUTPUT);
    digitalWrite(pins.pin1, LOW);
    digitalWrite(pins.pin2, LOW);
  }
  // LCD has 4 lines with 20 chars
  lcd.begin(NUM_CHAR, NUM_LINES);
  lcd.setCursor(0, 0);
  lcd.print("Analog 3:");
  lcd.setCursor(0, 1);
  lcd.print("turn-rate:");
  lcd.setCursor(0, 2);
  // lcd.print("heading:");
  lcd.print("target:");
  lcd.setCursor(0, 3);
  lcd.print("heading: xxxdeg");
  calibrateTurnRate();
}

void loop(){
  // Aufgabe 1
  uint16_t adcValue;
  // read the current ADC value on A3
  adcValue = getADC();
  lcd.setCursor(10, 0);
  lcd.print(adcValue);

  // Aufgabe 2
  uint16_t turn_rate;
  // calculate turn-rate
  turn_rate = adcValue - no_turn;
  lcd.setCursor(11, 1);
  lcd.print(turn_rate);

  // Aufgabe 3
  unsigned long thisTime;
  static unsigned long lastTime = 0;
  uint32_t heading_int;
  // calculate direction
  thisTime = millis();
  unsigned long interval = thisTime - lastTime;
  lastTime = thisTime;
  // deadband for noise
  if (abs(turn_rate) > 100){ // replace 100 with appropriate limit
    heading_int += turn_rate * interval;
  }
  lcd.setCursor(9, 2);
  lcd.print(heading_int);

  // Aufgabe 4
  uint32_t scaled_int;
  // scale heading_int
  // scaling_factor =
  // scaled_int = (heading_int * scaling_factor) % 360;
  lcd.setCursor(9, 3);
  lcd.print(scaled_int);

  // Aufgabe 6 + 7
  uint8_t state = 0;
  const uint8_t epsilon = 2;
  static unsigned long recentTime = 0;
  uint16_t targetHeading = 0;
  unsigned long currentTime = millis();
  if (currentTime - recentTime < 4000 && state == 0){
    // setMotor(Motor::A, true, 2000);
    // setMotor(Motor::B, true, 2000);
  }
  else{
    targetHeading = heading_int + 120;
    if (targetHeading > 359){
      targetHeading = targetHeading % 360;
    lcd.setCursor(8, 2);
    lcd.print(targetHeading);
    state = 1;
    }
    while (heading_int != targetHeading){
      // setMotor(Motor::A, true, speed);
      // setMotor(Motor::B, false, speed);
      if (heading_int <= targetHeading - epsilon || heading_int >= targetHeading + epsilon){
        // digitalWrite(pins.pin1, LOW);
        ;
      }
    // }
    state = 0;
    recentTime = currentTime;
  }
}


/* Useful LCD functions:
set the current write position of the lcd to specific line and row:
  lcd.setCursor(row, line);  

write onto lcd, starting at current position:
  lcd.print("abc");
*/