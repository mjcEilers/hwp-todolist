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

// We calculated the heading scaling factor by spinning the s-trike
// 10 times, dividing the accumulated heading_int by 10
#define GYRO_FULL_TURN 1140091

#define MAX_SPEED 65535
#define DRIVE_SPEED MAX_SPEED / 20
#define MAX_TURN_SPEED MAX_SPEED / 10
#define MIN_TURN_SPEED MAX_SPEED / 22

// State for exercise 06
enum State {
  DRIVE,
  TURN,
};

State state = DRIVE;

enum Motor {
  A,
  B
};

enum Direction {
  STOP,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

struct MotorPins {
  uint8_t pin1;
  uint8_t pin2;
};

const MotorPins motorPins[2] = {
  {PIN_MOTOR_A1, PIN_MOTOR_A2},
  {PIN_MOTOR_B2, PIN_MOTOR_B1}
};

/// returns true if INPUT is in [TARGET - EPSILON, TARGET + EPSILON] and else false
bool isDegClose(int16_t input, int16_t target, uint16_t epsilon) {
  uint16_t diff = abs(input - target) % 360;
  if(diff > 180) {
    diff = 360 - diff;
  }
  return diff <= epsilon;
}

// Aufgabe 1
int16_t getADC(){
  return analogRead(SENSOR_A3);
}

// Aufgabe 2
int16_t no_turn = 0;
void calibrateTurnRate(){
  delay(100);
  int32_t add = 0;
  for (int8_t i = 0; i < 100; i++){
    add += getADC();
    delay(10);
  }
  no_turn = add / 100;
}

void stopMotor(Motor motor) {
  MotorPins pins = motorPins[motor];
  digitalWrite(pins.pin1, LOW);
  digitalWrite(pins.pin2, LOW);
}

// Aufgabe 5
// Forward: A1 <- LOW; A2 <- HIGH
//          B2 <- LOW; B1 <- HIGH
//
// Backward: A1 <- HIGH; A2 <- LOW
//           B2 <- HIGH; B1 <- LOW
void setMotor(Motor motor, bool forward, int16_t speed){
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

/// Set both motors, specifying the direction to take and speed to drive
void setMotors(Direction direction, int16_t speed) {
  switch (direction) {
    case STOP:
      stopMotor(A);
      stopMotor(B);
      break;

    case FORWARD:
      setMotor(A, true, speed);
      setMotor(B, true, speed);
      break;

    case BACKWARD:
      setMotor(A, false, speed);
      setMotor(B, false, speed);
      break;

    case LEFT:
      setMotor(A, false, speed);
      setMotor(B, true, speed);
      break;

    case RIGHT:
      setMotor(A, true, speed);
      setMotor(B, false, speed);
      break;
  }
}

// initialization
void setup(){
  for (MotorPins pins : motorPins){
    pinMode(pins.pin1, OUTPUT);
    pinMode(pins.pin2, OUTPUT);
  }
  setMotors(STOP, 0);

  // LCD has 4 lines with 20 chars
  lcd.begin(NUM_CHAR, NUM_LINES);
  lcd.setCursor(0, 0);
  lcd.print("Analog 3:");
  lcd.setCursor(0, 1);
  lcd.print("turn-rate:");
  lcd.setCursor(0, 2);
  // lcd.print("heading:");
  lcd.print("target: xxxdeg");
  lcd.setCursor(0, 3);
  lcd.print("heading: xxxdeg");
  calibrateTurnRate();
}

bool first_run = true;
void loop(){
  // Aufgabe 1
  int16_t adcValue;
  // read the current ADC value on A3
  adcValue = getADC();
  lcd.setCursor(10, 0);
  lcd.print("         ");
  lcd.setCursor(10, 0);
  lcd.print(adcValue);

  // Aufgabe 2
  int16_t turn_rate;
  // calculate turn-rate
  turn_rate = adcValue - no_turn;
  lcd.setCursor(11, 1);
  lcd.print("        ");
  lcd.setCursor(11, 1);
  lcd.print(turn_rate);

  // Aufgabe 3
  static unsigned long last_time = 0;
  static int32_t heading_int = 0;
  // calculate direction
  unsigned long current_time = millis();
  unsigned long interval = current_time - last_time;
  last_time = current_time;
  // deadband for noise
  if (abs(turn_rate) > 40){
    heading_int += turn_rate * interval;
  }

  // display heading_int in 3rd line of lcd
  // lcd.setCursor(9, 2);
  // lcd.print("          ");
  // lcd.setCursor(9, 2);
  // lcd.print(heading_int);

  // Aufgabe 4
  // heading in degrees
  int16_t heading = ((heading_int * 360)/ GYRO_FULL_TURN) % 360;
  lcd.setCursor(9, 3);
  lcd.print("   ");
  lcd.setCursor(9, 3);
  lcd.print(heading);

  // Aufgabe 6 + 7
  // we tested driving on laminate ground
  // Increase *SPEED defines for rougher ground!

  // we defined state as an enum rather than an integer
  // casting to int comes down to the same though :)
  static State state = DRIVE;

  const uint8_t epsilon = 2;
  static int16_t targetHeading = 0;

  // timer
  static unsigned long recentTime = 0;
  // for the first iteration to actually drive 4s
  if (first_run) {
    recentTime = millis();
    setMotors(FORWARD, DRIVE_SPEED);
    first_run = false;
  }
  uint16_t timer = (millis() - recentTime) / 1000;

  // display targetHeading in 3rd line of the lcd
  lcd.setCursor(8, 2);
  lcd.print("   ");
  lcd.setCursor(8, 2);
  lcd.print(targetHeading);

  // state transitions
  switch (state) {
    case DRIVE:
      // DRIVE -> TURN
      if (timer >= 4) {
        state = TURN;
        targetHeading = (heading + 120) % 360;
        setMotors(LEFT, MAX_TURN_SPEED);
      }
      break;

    case TURN:
      // TURN -> DRIVE
      if (isDegClose(heading, targetHeading, epsilon)) {
        state = DRIVE;
        recentTime = millis();
        setMotors(FORWARD, DRIVE_SPEED);
      }
      // slow down turning, the closer the robot becomes to the target angle
      else {
        uint16_t diff = abs(targetHeading - heading) % 360;
        diff = (diff > 180) ? 360 - diff : diff;
        uint16_t turn_speed = map(diff, 0, 180, MIN_TURN_SPEED, MAX_TURN_SPEED);
        setMotors(LEFT, turn_speed);
      }
      break;
  }
}


/* Useful LCD functions:
set the current write position of the lcd to specific line and row:
  lcd.setCursor(row, line);  

write onto lcd, starting at current position:
  lcd.print("abc");
*/