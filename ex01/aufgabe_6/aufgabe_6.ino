 #define SERIAL_COMSPEED 9600

void setup() {
  Serial.begin(SERIAL_COMSPEED);
  pinMode(A0, INPUT);
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

void loop() {
  int a0_read = analogRead(A0);
  int state = getButtonI(a0_read);

  if (state > 0){
    Serial.print(state);
  }
  else{
    Serial.print("");
  }
}
