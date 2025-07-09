#define SERIAL_SPEED 38400
#define MAX_MELODY_LEN 100

volatile uint32_t tCount = 0;
volatile uint8_t melodyIdx = 0;

// uint16_t durations[10] = {210, 290, 350, 405, 485, 516, 543, 603, 760, 963};
// uint16_t notes[10] = {308, 433, 583, 597, 620, 680, 713, 780, 873, 1000};

char buffer[] = "Test:d=4,o=5,b=200:8g,8a,8c6,8a,e6,8p,e6,8p,d6.,p,8p,8g,8a,8c6,8a,d6,8p,d6,8p,c6,8b,a.,8g,8a,8c6,8a,2c6,d6,b,a,g.,8p,g,2d6,2c6.";

// source for note freq is https://de.wikipedia.org/wiki/Frequenzen_der_gleichstufigen_Stimmung
// here A in our enum is the kammerton with frequenz 440
enum BASE {
  A = 440,
  As = 466,
  B = 494,
  C = 523,
  Cs = 554,
  D = 587,
  Ds = 622,
  E = 659,
  F = 698,
  Fs = 740,
  G = 784,
  Gs = 831,
  P = 0
};

struct Paras {
  uint16_t duration;
  uint16_t octave;
  uint16_t bpm;
};

struct Melody {
  uint16_t durations[MAX_MELODY_LEN];
  uint16_t notes[MAX_MELODY_LEN];
  uint16_t len;
};

struct Melody test = {{210, 290, 350, 405, 485, 516, 543, 603, 760, 963}, {308, 433, 583, 597, 620, 680, 713, 780, 873, 1000}, 10};

// returns weather the given char C is a digit or not
bool charIsDigit(char c) {
  if (c < '0' || c > '9') return false;
  return true;
}

// Takes char* BUFFER and START index and converts BUFFER into an int until non digit is next char (returns -1 if no digit is found at start)
int str2int(char* buffer, uint16_t start) {
  if(!charIsDigit(buffer[start])) return -1;
  uint16_t val = 0;
  uint16_t curr_index = start;
  while (charIsDigit(buffer[curr_index])) {
    val *= 10;
    val += buffer[curr_index] - '0';
    curr_index++;
  }
  return val;
}


// Returns the distance from the current index START to the next char that equals TARGET in BUFFER
uint16_t next_char(char* buffer, char target, int start) {
  uint16_t idx = start;
  while (idx < strlen(buffer)) {
    if (buffer[idx] == target) {
      return idx - start;
    }
    idx++;
  }
  return -1;
}

struct Paras parseParas(char* melody_str) {
  struct Paras parameters;
  // setting standart values
  parameters.duration = 4;
  parameters.octave = 6;
  parameters.bpm = 63;
  uint16_t parsing_index = 0;
  parsing_index = next_char(melody_str, ':', parsing_index) + 1;
  if(melody_str[parsing_index] == 'd') {
    parsing_index += 2;
    parameters.duration = str2int(melody_str, parsing_index);
  }
  // gets index for next parameter (o) if not skipp to next parameter
  uint16_t next_para_index = next_char(melody_str, 'o', parsing_index);
  if(next_para_index >= 0) {
    parsing_index += next_para_index + 2;
    parameters.octave = str2int(melody_str, parsing_index);
  }
  next_para_index = next_char(melody_str, 'b', parsing_index);
  if(next_para_index >= 0) {
    parsing_index += next_para_index + 2;
    parameters.bpm = str2int(melody_str, parsing_index);
  }
  return parameters;
}

// Parses a melody string into a melody struct
struct Melody parseMelody(char* melody_str, uint16_t melody_len) {
  struct Melody ret;
  ret.len = melody_len;
  if (melody_len == 0) {
    return ret;
  }
  int parsing_index = 0;
  int buffer_len = strlen(melody_str);
  // skip titel
  parsing_index += next_char(melody_str, ':', parsing_index) + 1;
  // standart default parameters
  int duration = 4;
  int octave = 6;
  int bpm = 63;
  // read default parameters
  if(melody_str[parsing_index] == 'd') {
    parsing_index += 2;
    duration = str2int(melody_str, parsing_index);
    parsing_index += next_char(melody_str, ',', parsing_index) + 1;
  }
  if(melody_str[parsing_index] == 'o') {
    parsing_index += 2;
    octave = str2int(melody_str, parsing_index);
    parsing_index += next_char(melody_str, ',', parsing_index) + 1;
  }
  if(melody_str[parsing_index] == 'b') {
    parsing_index += 2;
    bpm = str2int(melody_str, parsing_index);
    parsing_index += next_char(melody_str, ',', parsing_index) + 1;
  }
  Serial.print(duration);
  Serial.print("\n");
  Serial.print(octave);
  Serial.print("\n");
  Serial.print(bpm);
  Serial.print("\n");
  return;
  // parse notes until melody finished
  int note_index = 0;

  do {
    parsing_index++;
    // single note parse
    //reads duration (its always at the start)
    uint16_t note_duration = str2int(melody_str,parsing_index);
    // if no duration is specified (returned as -1) use standart duration
    if (note_duration == -1) note_duration = duration;
    // caclulate time in millis
    ret.durations[note_index] = duration * 60 * 1000 / (note_duration * bpm);
    // skip time specifcation
    while(charIsDigit(melody_str[parsing_index])) parsing_index++;
    // read note
    BASE note;
    switch (melody_str[parsing_index]) {
      case 'c': 
        note = (melody_str[parsing_index + 1] == "#") ? Cs : C;
        break;

      case 'd': 
        note = (melody_str[parsing_index + 1] == "#") ? Ds : D;
        break;

      case 'e':
        note = E;
        break;

      case 'f':
        note = (melody_str[parsing_index + 1] == "#") ? Fs : F;
        break;

      case 'g':
        note = (melody_str[parsing_index + 1] == "#") ? Gs : G;
        break;

      case 'a':
        note = (melody_str[parsing_index + 1] == "#") ? As : A;
        break;

      case 'b':
        note = B;
        break;

      case 'p':
        note = P;
        break;

      default:
        note = P;
        break;
    }

    parsing_index += (melody_str[parsing_index + 1] == '#') ? 2 : 1;

    // Read octave
    uint16_t note_octave = (charIsDigit(melody_str[parsing_index])) ? str2int(melody_str, parsing_index) : octave;
    ret.notes[note_index] = ((uint16_t) note) * pow(2, note_octave);
    while (charIsDigit(melody_str[parsing_index])) parsing_index++;

    if (melody_str[parsing_index] == '.') {
      ret.durations[note_index] *= 1.5;
      parsing_index++;
    }

    note_index++;    

  } while (melody_str[parsing_index] == ',');
  return ret;
}

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
  // Test aufgabe 4
  // setTimer1Freq(200);
  // Test aufgabe 5
  // setTimer2(true);
  //playMelody();
  struct Paras parameters = parseParas(buffer);
  // Serial.print(parameters.duration);
  // Serial.print("\n");
  // Serial.print(parameters.octave);
  // Serial.print("\n");
  // Serial.print(parameters.bpm);
  // Serial.print("\n");
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
//   cli();
//   //Reset control registers
//   TCCR1A = 0;
//   TCCR1B = 0;
//   TIMSK1 = 0;
//   TCNT1 = 0;
//   // Set clock prescaler 8
//   TCCR1B |= (1 << CS11);
//   // Set mode to CTC
//   TCCR1B |= (1 << WGM12); // Warum liegen WGM12 und WGM13 nicht im selben Register?
//   // Set output compare register
//   // 8MHz / (2 * 8 * (477+1)) = 1046Hz (datasheet S. 123)
//   // 8MHz / (2 * 64 * (58+1)) = 1059Hz
//   // -> if prescaler is lower, it gets closer to frequency 1046Hz
//   OCR1A = 477;
//   // Enable interrupt on compare match (calls interrupt function)
//   TIMSK1 |= (1 << OCIE1A);
//   sei();
// }
// 
// ISR(TIMER1_COMPA_vect){
//   PINB |= (1 << PB4);
// }

// Aufgabe 3
// void setTimer1Freq(){
//   cli();
//   TCCR1A = 0;
//   TCCR1B = 0;
//   TIMSK1 = 0;
//   TCNT1 = 0;
//   // Set clock prescaler 8
//   TCCR1B |= (1 << CS11);
//   // Set mode to CTC
//   TCCR1B |= (1 << WGM12);
//   // Set output compare register
//   OCR1A = 477;
//   // Toggle OC1A on compare match (datasheet S.131)
//   TCCR1A |= (1 << COM1B0);
//   sei();
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
    TCCR1A |= (1 << COM1B0);
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
    // Disable interrupt on compare match
    TIMSK2 &= ~(1 << OCIE2A);
    // No clock source. Timer stopped. (datasheet S.156)
    TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
  }
}
// Aufgabe 6
ISR(TIMER2_COMPA_vect){
  tCount++;
  if (tCount >= test.durations[melodyIdx]){
    tCount = 0;
    melodyIdx++;
    if (melodyIdx <= 9){
      Serial.print(melodyIdx);
      Serial.print("\n");
      setTimer1Freq(test.notes[melodyIdx]);
    }
    else{
      setTimer2(false);
      // Turn of sound
      setTimer1Freq(0);
    }
  }
}
// Aufgabe 6
void playMelody(){
  melodyIdx = 0;
  tCount = 0;
  setTimer1Freq(test.notes[melodyIdx]);
  setTimer2(true);
}

uint16_t prevtCount = 0;

void loop() {
  // Aufgabe 1
  setPin13(true);
  delay(500);
  setPin13(false);
  delay(500);
  // Aufgabe 5
  // setTimer1Freq(261);
  // delay(300);
  // setTimer1Freq(293);
  // delay(300);
  // setTimer1Freq(329);
  // delay(300);
  // prints tCount every 1s/every 1000ms
  if (tCount - prevtCount > 1000){
    Serial.print(tCount);
    prevtCount = tCount;
  }
}
