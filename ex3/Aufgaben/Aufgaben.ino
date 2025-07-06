#define SERIAL_SPEED 115200
#define TIMER_INTERVAL 255

void setup() {
  Serial.beginn(SERIAL_SPEED);
  DDRB |= (1 << PB5); //defines Pin 13 as output
  //PORTB is register, Pin 13 is fifth bit of PORTB
  PORTB |= (1 << PB5); //initialises PORTB
  DDRB |= (1 << PB4); //defines Pin 12 as output
}

void setPin13(bool high){
  if (high == true){
    PORTB |= (1 << PB5);
  }
  else{
    PORTB &= ~(1 << PB5);
  }
}

void setTimer1Freq(){
  cli();
  // Reset control registers
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  // set clock prescaler 1024
  TCCR1B |= (1 << CS12)|(1 << CS11)|(1 << CS10);
  // set mode to CTC
  TCCR1A |= (1 << WGM11);
  // define x to be timer in millis
  OCR1A = TIMER_INTERVAL;
  // enable interrupt on compair match (calls interrupt function?)
  TIMSK1 |= (1 << OCIE1A);
  sei();
}

ISR(TIMER1_COMPA_vect){
  Serial.println("Heurika");
  PINB |= (PB4);
}

void loop() {
  setPin13(True);
  delay(500);
  setPin13(False);
  delay(500);
}
