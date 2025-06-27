
#define Motor_A1 D1
#define Motor_A2 D2
#define Motor_B1 D5
#define Motor_B2 D6
#define Ultrasonic D8


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Motor_A1, OUTPUT);
  pinMode(Motor_A2, OUTPUT);
  pinMode(Motor_B1, OUTPUT);
  pinMode(Motor_B2, OUTPUT);
}
//Aufgabe 1
//void setMotor(bool forward){
//  if (forward == True){
//    digitalWrite(Motor_A1, LOW);
//    digitalWrite(Motor_A2, HIGH);
//  else{
//    digitalWrite(Motor_A1, HIGH);
//    digitalWrite(Motor_A2, LOW);
//  }
//}

void setMotor(bool forward, uint16_t speed, uint8_t selectMotor){
  if (selectMotor == 1){
    if (forward == True){
      analogWrite(Motor_A1, 0);
      analogWrite(Motor_A2, speed);
    else{
      analogWrite(Motor_A1, speed);
      analogWrite(Motor_A2, 0);
  }
  else if (selectMotor == 2){
    if (forward == True){
      analogWrite(Motor_B1, speed);
      analogWrite(Motor_B2, 0);
    else{
      analogWrite(Motor_B1, 0);
      analogWrite(Motor_B2, speed);
  }
  else if (selectMotor == 3){
    if (forward == True){
      analogWrite(Motor_A1, 0);
      analogWrite(Motor_A2, speed);
      analogWrite(Motor_B1, speed);
      analogWrite(Motor_B2, 0);
    }
    else{
      analogWrite(Motor_A1, speed);
      analogWrite(Motor_A2, 0);
      analogWrite(Motor_B1, 0);
      analogWrite(Motor_B2, speed);
    }
  }
  else if (selectMotor == 4){
    analogWrite(Motor_A1, 0);
    analogWrite(Motor_A2, 0);
  }
  else if (selectMotor == 5){
    analogWrite(Motor_B1, 0);
    analogWrite(Motor_B2, 0);
  }
  else if (selectMotor == 6){
    analogWrite(Motor_A1, 0);
    analogWrite(Motor_A2, 0);
    analogWrite(Motor_B1, 0);
    analogWrite(Motor_B1, 0);
  }
  
}

void drive(bool flag, unsigned long time, uint16_t speed){
  long beginning_time = millis();
  if (flag == True){
    setMotor(True, speed, 1);
  }
  else{
    setMotor(False, speed, 1);
  }
  while True(){
    long current_time = millis();
    if (time < current_time - beginning_time){
      setMotor(True, speed, 6);
      break
  }
  delay(time);
  }
}

void turn(bool flag, unsigned long time, uint16_t speed){
  //Linkskurve
  if (flag == True){
    setMotor(True, speed, 1);
    setMotor(True, speed, 5);
  }
  //Rechtskurve
  else{
    setMotor(True, speed, 2);
    setMotor(True, speed, 4);
  }
  while True(){
    long current_time = millis();
    if (time < current_time - beginning_time){
      setMotor(True, speed, 6);
      break
  }
  delay(time);
}

int measureDistance(char pin){
  //Initialisierung
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
  pinMode(pin, INPUT);

  unsigned long result = pulseIn(pin, HIGH);
  Serial.print(result);
}

void loop() {
  // put your main code here, to run repeatedly:

}
