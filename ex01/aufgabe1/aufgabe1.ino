#define LED_PIN 10
#define DELAY_TIME 1000

long last_switch = 0;
bool mode = false;

void setup() {
  // put your setup code here, to run once:
  last_switch = millis();
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // I)
  // digitalWrite(LED_PIN, HIGH);
  // delay(DELAY_TIME);
  // digitalWrite(LED_PIN, LOW);
  // delay(DELAY_TIME);
  // II)
  long current_time = millis();
  if (current_time - last_switch > DELAY_TIME) {
    last_switch = current_time;
    mode = !mode;
    digitalWrite(LED_PIN, mode);
  }
  //Anmerkung: potentiell nicht overflow sicher
}
