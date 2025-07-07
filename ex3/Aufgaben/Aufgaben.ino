#define SERIAL_SPEED 115200

volatile uint32_t tCount = 0;

void setup() {
  Serial.begin(SERIAL_SPEED);
  //defines Pin 13 as output
  DDRB |= (1 << PB5);
  //PORTB is register, Pin 13 is fifth bit of PORTB
  //initialises Pin 13 with 1
  PORTB |= (1 << PB5);
  //defines Pin 12 as output
  DDRB |= (1 << PB4);
  //define Pin 10 as output
  DDRB |= (1 << PB2);
  //initialises Pin 10 with 0
  PORTB &= ~(1 << PB2);
  setTimer1Freq();
  setTimer2(true);
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
  // TCNT1 = 0;
  // Set clock prescaler 8
  // TCCR1B |= (1 << CS11);
  // Set mode to CTC
  // TCCR1B |= (1 << WGM12); // Warum liegen WGM12 und WGM13 nicht im selben Register?
  // Set output compare register
  // 8MHz / (2 * 8 * (477+1)) = 1046Hz (datasheet S. 123)
  // 8MHz / (2 * 64 * (58+1)) = 1059Hz
  // -> if prescaler is lower, it gets closer to frequency 1046Hz
  // OCR1A = 477;
  // Enable interrupt on compare match (calls interrupt function)
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
  // TCNT1 = 0;
  // Set clock prescaler 8
  // TCCR1B |= (1 << CS11);
  // Set mode to CTC
  // TCCR1B |= (1 << WGM12);
  // Set output compare register
  // OCR1A = 477;
  // Toggle OC1A on compare match (datasheet S.131)
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
    TCNT1 = 0;
    // Set clock prescaler 8
    TCCR1B |= (1 << CS11);
    // Set mode to CTC
    TCCR1B |= (1 << WGM12);
    // Set output compare register
    OCR1A = (8000000 / (2 * 8 * freq)) - 1;
    // Toggle OC1A on compare match (datasheet S.131)
    TCCR1A |= (1 << COM1A0);
    sei();
  }
  else{
    PORTB &= ~(1 << PB2);
    // No clock source. Timer stopped. (datasheet S.133)
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
  }
}

//Aufgabe 5
void setTimer2(bool timer){
  if (timer){
    cli();
    // Reset control register
    TCCR2A = 0;
    TCCR2B = 0;
    TIMSK2 = 0;
    TCNT2 = 0;
    // Set clock prescaler 32
    TCCR2B |= (1 << CS21) | (1 << CS20);
    // Set mode to CTC
    TCCR2A |= (1 << WGM21);
    // Set output compare register match
    // 8MHz / (32 * (249+1)) = 1000Hz = 1ms
    OCR2A = 249;
    // Enable interrupt on compare match
    TIMSK2 |= (1 << OCIE2A);
    sei();
  }
  else{
    // disable interrupt
    TIMSK2 &= ~(1 << OCIE2A);
    // No clock source. Timer stopped. (datasheet S.156)
    TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
  }
}

ISR(TIMER2_COMPA_vect){
  tCount++;
}


void loop() {
  // Aufgabe 1
  setPin13(true);
  delay(500);
  setPin13(false);
  delay(500);
  // Aufgabe 5
  setTimer1freq(261);
  delay(300);
  setTimer1freq(293);
  delay(300);
  setTimer1freq(329);
  delay(300);

  Serial.print(tCount);
}
