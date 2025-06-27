#define US1_PIN D8
#define US_TIMEOUT_US 30000

#define MOTOR_A1_PIN D1
#define MOTOR_A2_PIN D2
#define MOTOR_B1_PIN D5
#define MOTOR_B2_PIN D6

enum Motor {
  A,
  B
};

struct MotorPins {
  uint8_t pin1;
  uint8_t pin2;
};

const MotorPins motorPins[2] = {
  {MOTOR_A1_PIN, MOTOR_A2_PIN},
  {MOTOR_B2_PIN, MOTOR_B1_PIN}
};

void setup() {
  Serial.begin(115200);

  for ( MotorPins pins : motorPins ) {
    pinMode(pins.pin1, OUTPUT);
    pinMode(pins.pin2, OUTPUT);
    digitalWrite(pins.pin1, LOW);
    digitalWrite(pins.pin2, LOW);
  }

  pinMode(US1_PIN, OUTPUT);
  digitalWrite(US1_PIN, LOW);
}

void loop() {
  static char buf[8];
  // drive(true, 2000, 2000);
  // delay(500);
  // drive(false, 1000, 2000);
  // delay(500);
  // turn(true, 500, 2000);
  // delay(500);
  // turn(false, 500, 2000);
  // delay(10000);
  float us1_m = measureDistance(US1_PIN);
  Serial.println(dtostrf(us1_m, 4, 2, buf));
  delay(100);
}

void setMotor(Motor motor, bool forward, uint16_t speed) {
  uint16_t speed_10 = map(speed, 0, 65535, 0, 1023);  // map 16bit to 10bit value
  MotorPins pins = motorPins[motor];

  switch (forward) {
    case true:
      // drive forward
      analogWrite(pins.pin1, LOW);
      analogWrite(pins.pin2, speed_10);
      break;

    case false:
      // drive backward
      analogWrite(pins.pin1, speed_10);
      analogWrite(pins.pin2, LOW);
      break;
  }
}

void stopMotor(Motor motor) {
  MotorPins pins = motorPins[motor];
  digitalWrite(pins.pin1, LOW);
  digitalWrite(pins.pin2, LOW);
}

void drive(bool forward, unsigned long duration_ms, uint16_t speed) {
  setMotor(Motor::A, forward, speed);
  setMotor(Motor::B, forward, speed);
  delay(duration_ms);

  stopMotor(Motor::A);
  stopMotor(Motor::B);
}

void turn(bool left, unsigned long duration_ms, uint16_t speed) {
  setMotor(Motor::A, left, speed);
  setMotor(Motor::B, !left, speed);
  delay(duration_ms);

  stopMotor(Motor::A);
  stopMotor(Motor::B);
}

float measureDistance(uint8_t us_pin) {
  const float SPEED_OF_SOUND = 0.000343;

  // Send trigger
  pinMode(us_pin, OUTPUT);
  digitalWrite(us_pin, LOW);
  delayMicroseconds(1);
  digitalWrite(us_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(us_pin, LOW);
  
  // Measure response
  pinMode(us_pin, INPUT);
  unsigned long pulseWidth = pulseIn(us_pin, HIGH, US_TIMEOUT_US);

  // Return -1 if timeout
  if (pulseWidth == 0) return -1.0f;

  // Calculate distance
  return pulseWidth * SPEED_OF_SOUND / 2.;
}