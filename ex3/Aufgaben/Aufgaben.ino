#define SERIAL_SPEED 115200

void setup() {
  Serial.begin(SERIAL_SPEED);
  DDRB |= (1 << PB5); //defines Pin 13 as output
  //PORTB is register, Pin 13 is fifth bit of PORTB
  PORTB |= (1 << PB5); //initialises Pin 13 with 1
  DDRB |= (1 << PB4); //defines Pin 12 as output
  DDRB |= (1 << PB2); //define Pin 10 as output
  PORTB &= ~(1 << PB2); //initialises Pin 10 with 0
  setTimer1Freq();
}

// Aufgabe 1
void setPin13(bool high){
  if (high == true){
    PORTB |= (1 << PB5);
  }
  else{
    PORTB &= ~(1 << PB5);
  }
}

// Aufgabe 2
// void setTimer1Freq(){
  // cli();
  // Reset control registers
  // TCCR1A = 0;
  // TCCR1B = 0;
  // TIMSK1 = 0;
  // set clock prescaler to 8
  // TCCR1B |= (1 << CS11);
  // set mode to CTC
  // TCCR1B |= (1 << WGM12); // Warum liegen WGM12 und WGM13 nicht im selben Register?
  // set output compare register
  // 8MHz / (2 * 8 * (477+1)) = 1046Hz (datasheet S. 123)
  // 8MHz / (2 * 64 * (58+1)) = 1059Hz
  // -> if prescaler is lower, it gets closer to frequency 1046Hz
  // OCR1A = 477;
  // enable interrupt on compare match (calls interrupt function)
  // TIMSK1 |= (1 << OCIE1A);
  // sei();
// }

// ISR(TIMER1_COMPA_vect){
  // Serial.println("Heurika");
  // PINB |= (1 << PB4);
// }

// Aufgabe 3
// void setTimer1Freq(){
  // cli();
  // Reset control registers
  // TCCR1A = 0;
  // TCCR1B = 0;
  // TIMSK1 = 0;
  // set clock prescaler to 8
  // TCCR1B |= (1 << CS11);
  // set mode to CTC
  // TCCR1B |= (1 << WGM12);
  // set output compare register
  // OCR1A = 477;
  // Toggle OC1A on Compare Match (datasheet S.131)
  // TCCR1A |= (1 << COM1A0);
  // sei();
// }

//Aufgabe 4
void setTimer1Freq(uint16_t freq){
  if (freq >= 100 && freq <= 3000){
    cli();
    // Reset control registers
    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 = 0;
    // set clock prescaler to 8
    TCCR1B |= (1 << CS11);
    // set mode to CTC
    TCCR1B |= (1 << WGM12);
    // set output compare register
    OCR1A = (8000000 / (2 * 8 * freq)) - 1;
    // Toggle OC1A on Compare Match (datasheet S.131)
    TCCR1A |= (1 << COM1A0);
    sei();
  }
  else{
    PORTB &= ~(1 << PB2);
    // No clock source. Timmer stopped. (datasheet S.133)
    TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10);
  }
}

void loop() {
  return;
  setPin13(true);
  delay(500);
  setPin13(false);
  delay(500);
}
