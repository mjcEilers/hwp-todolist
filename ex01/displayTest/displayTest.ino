// Include LCD functions:
#include <LiquidCrystal.h>

// General Definitions
#define REFERENCE_VOLTAGE 3.3
#define MAX_READ 1023

// ----- Pin definitions -----
const int ledPins[6] = { 2, 3, 4, 5, 6, 7 };  // GGYYRR
#define TR1_PIN A0
#define BUTTON_PIN A1

#define R_S 8
#define E 9
#define DB4 10
#define DB5 11
#define DB6 12
#define DB7 13

// LCD Defines
#define NUM_CHAR 20
#define NUM_LINES 4

// enum declarations; wanted to structure differently, but had issues compiling :')

// State of the LEDs
// Eigher a blinking color pair
// or one active LED running back and forth
enum State {
  GREEN,
  YELLOW,
  RED,
  RUNNING
};

State ledState = State::GREEN; // Global state

// Labels used on LCD
// exactly 3 letters to make printing easy
const char* stateLabel[4] = { "GRN", "YLW", "RED", "RUN" };

// State of the 5-button setup of the s-trike
enum Button : char {
  NONE = '-',
  S1 = '1',
  S2 = '2',
  S3 = '3',
  S4 = '4',
  S5 = '5',
};

const unsigned long MIN_SWITCH_DELAY = 300;

// Handles switching ledState upon button press
// returns whether the state switched or not
bool handleStateSwitch() {
  static unsigned long lastSwitch = 0;

  unsigned long currentTime = millis();
  if (getButtonDeprelled() == Button::S1 && currentTime - lastSwitch > MIN_SWITCH_DELAY) {
    ledState = (State)(((int)ledState + 1) % 4);
    lastSwitch = currentTime;
    return true;
  }
  
  return false;
}

int runningIndex = 0;
bool blinkOn = true;

// Handles led transition based on set frequency
// returns whether the LEDs are in transition or not
bool handleLedTransition(int frequency) {
  static unsigned long lastTransition = 0;
  static bool runningForward = true;

  long transitionInterval = 1000 / (2 * frequency);
  unsigned long currentTime = millis();

  if (currentTime - lastTransition > transitionInterval) {
    lastTransition = currentTime;
    blinkOn = !blinkOn;  // Switch blinking

    // Run the lights back and forth
    if (runningForward) { runningIndex++; }
    else { runningIndex--; }

    if (runningIndex == 0) { runningForward = true; }
    if (runningIndex == 5) { runningForward = false; }

    return true;
  }

  return false;
}

// Read the potentiometer TR1 and map the value to a frequency
// returns a frequency in range [1, 50] Hz
int getFrequency() {
  int raw = analogRead(TR1_PIN);
  return map(raw, 0, MAX_READ, 1, 50);
}

// Read the potentiometer TR1 and return its outgoing voltage
float getVoltage() {
  int raw = analogRead(TR1_PIN);
  return raw * REFERENCE_VOLTAGE / MAX_READ;
}

// Read the S-Trikes 5-button setups analog signal
// and map the pressed button accordingly
// returns the pressed button (instable)
Button getButton() {
  // S1: 0
  // S2: 242
  // S3: 473
  // S4: 677
  // S5: 830
  int value = analogRead(BUTTON_PIN);

  if (value < 121) { return Button::S1; }
  if (value < 357) { return Button::S2; }
  if (value < 575) { return Button::S3; }
  if (value < 753) { return Button::S4; }
  if (value < 926) { return Button::S5; }

  return Button::NONE;
}

const unsigned long DEBOUNCE_DELAY = 30;

// Handle the button press using deprelling
// returns the last stable button press
Button getButtonDeprelled() {
  static Button lastRead = NONE;
  static Button lastStable = NONE;
  static unsigned long lastDeprellTime = 0;

  Button current = getButton();

  if (current != lastRead) {
    lastDeprellTime = millis();
    lastRead = current;
  }

  // If a button is pressed long enough according to signal, consider it stable
  if ((millis() - lastDeprellTime) > DEBOUNCE_DELAY) {
    lastStable = current;
  }

  return lastStable;
}

// -------- LCD -------
LiquidCrystal lcd(R_S, E, DB4, DB5, DB6, DB7);

// Sets up the LCD
void setupDisplay() {
  // LCD has 4 lines with 20 chars
  lcd.begin(NUM_CHAR, NUM_LINES);
  lcd.print("Analog 0: x.xx V");
  lcd.setCursor(0, 1);
  lcd.print("Button: Sx");
  lcd.setCursor(0, 2);
  lcd.print("State: xxx");
  lcd.setCursor(0, 3);
  lcd.print("Frequency: xx Hz");
}

// Limit the displays refresh rate to avoid flickering
const unsigned long DISPLAY_UPDATE_DELAY = 20; // max. 50 Hz

// Updates the LCD
// Displays voltage on A0, active button, active state and frequency
void updateDisplay(float voltage, Button buttonState, State ledState, int frequency) {
  static unsigned long lastUpdate = 0;

  // skip update if refreshing prematurely
  if (millis() - lastUpdate < DISPLAY_UPDATE_DELAY) return;

  // Print voltage
  char v[4];
  dtostrf(voltage, 4, 2, v);
  lcd.setCursor(10, 0);
  lcd.print(v);

  // Print button state
  lcd.setCursor(9, 1);
  //lcd.print((char) buttonState);
  lcd.print((char) getButtonDeprelled());

  // Print LED state
  lcd.setCursor(7, 2);
  lcd.print(stateLabel[(int) ledState]);

  // Print frequency
  lcd.setCursor(11, 3);
  if (frequency < 10) lcd.print("0"); // Leading 0
  lcd.print(frequency);
}

void setup() {
  // Set Trimmer and Button Pins
  pinMode(TR1_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  // Set LED pins
  for (auto pin : ledPins) pinMode(pin, OUTPUT);

  // Setup the LCD
  setupDisplay();
}

void loop() {
  // Read sensors
  float voltage = getVoltage();
  Button buttonState = getButtonDeprelled();
  int frequency = getFrequency();

  // Display current state
  updateDisplay(voltage, buttonState, ledState, frequency);

  // Handle transitions
  bool _stateSwitched = handleStateSwitch();
  bool ledTransitioned = handleLedTransition(frequency);

  // Skip setting leds unless in transition
  if (!ledTransitioned) return;

  // Clear all LEDs
  for (auto led : ledPins) digitalWrite(led, LOW);

  // Set active LEDs according to state
  switch (ledState) {
    case State::GREEN:
      digitalWrite(ledPins[0], blinkOn);
      digitalWrite(ledPins[1], blinkOn);
      break;

    case State::YELLOW:
      digitalWrite(ledPins[2], blinkOn);
      digitalWrite(ledPins[3], blinkOn);
      break;

    case State::RED:
      digitalWrite(ledPins[4], blinkOn);
      digitalWrite(ledPins[5], blinkOn);
      break;

    case State::RUNNING:
      digitalWrite(ledPins[runningIndex], HIGH);
      break;
  }
}
