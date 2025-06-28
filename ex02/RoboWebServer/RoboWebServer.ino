// Include WiFi libs
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "website.h"

// Add your wifi credentials here
const char* ssid     = "SSID";
const char* password = "PASSWORD";

// Webserver on port 80 (standard http port)
WiFiServer server(80);

// Variable to store incoiming http request
String request;

// Name of the device (can be used as DNS query in browser)
#define DEVICE_NAME "HWPRobo"

// Motor defines
#define DRIVE_INTERVAL 300
#define MAX_MOTOR_SPEED 65535
#define DEFAULT_SPEED 40000
#define MOTOR_A1_PIN D1
#define MOTOR_A2_PIN D2
#define MOTOR_B1_PIN D5
#define MOTOR_B2_PIN D6
const uint8_t motorPins[] = {MOTOR_A1_PIN, MOTOR_A2_PIN, MOTOR_B1_PIN, MOTOR_B2_PIN};

// Us defines
#define US_TIMEOUT_US 30000
#define US1_PIN D8
#define US2_PIN D7
#define US3_PIN D3
const uint8_t usPins[] = {US1_PIN, US2_PIN, US3_PIN};

bool autoDriveActive = false;

void setup() {
  // Init serial
  Serial.begin(115200);

  // Init motor pins as output
  for (size_t i = 0; i < sizeof(motorPins)/sizeof(motorPins[0]); i++) {
    pinMode(motorPins[i], OUTPUT);
    digitalWrite(motorPins[i], LOW);
  }
  
  // Comment if you want that the ESP creates an AP
  // Connect to wifi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Die IP vom Webserver auf dem seriellen Monitor ausgeben
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP Adress: ");
  Serial.println(WiFi.localIP());

  // Uncomment if you want that the ESP creates an AP
  /*
  // You can remove the password parameter if you want the AP to be open. 
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  */

  // mDNS name resolving
  if (MDNS.begin(DEVICE_NAME)) {
    Serial.println("MDNS started");
  } else {
    Serial.println("Error starting MDNS");
  }

  // Start webserver
  server.begin();
}

void loop() {
  // Handle clients
  handleClient();
  // Update MDNS
  MDNS.update();
  // Handles auto drive if active
  handleAutoDrive();
}

void handleClient() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)  {  
    return;  
  }
  // Read the first line of the request (we just need this line)
  request = client.readStringUntil('\r');

  // Print request to serial
  Serial.print("request: ");
  Serial.println(request); 

  // print header message
  client.println(header);
  // Check for corresponding get message  
  if (request.indexOf("GET /pollUS") >= 0) {
    Serial.println("Polling");
    float us1, us2, us3;
    // Insert your code here
    us1 = measureDistance(US1_PIN);
    us2 = measureDistance(US2_PIN);
    us3 = measureDistance(US3_PIN);

    // Send US data to website
    client.printf("{\"US1\":%.2f, \"US2\":%.2f, \"US3\":%.2f}", us1, us2, us3);

  }

  else if (request.indexOf("GET /up") >= 0) {
    Serial.println("Driving forward");
    drive(true, DRIVE_INTERVAL, DEFAULT_SPEED);
    client.println("Success");
  }
  
  else if (request.indexOf("GET /back") >= 0) {
    Serial.println("Driving forward");
    drive(false, DRIVE_INTERVAL, DEFAULT_SPEED);
    client.println("Success");
  }

  else if (request.indexOf("GET /left") >= 0) {
    Serial.println("Driving forward");
    turn(true, DRIVE_INTERVAL, DEFAULT_SPEED);
    client.println("Success");
  }

  else if (request.indexOf("GET /right") >= 0) {
    Serial.println("Driving forward");
    turn(false, DRIVE_INTERVAL, DEFAULT_SPEED);
    client.println("Success");
  }else if (request.indexOf("GET /auto") >= 0) {
    if(autoDriveActive) {
      autoDriveActive = false;
      Serial.println("Stopped Auto Drive");
    }else {
      autoDriveActive = true;
      Serial.println("Started Auto Drive");
    }
    client.println("Success");
  }
  
  else {
    // Finish HTTP-Request with a newline (thats cruical)
    client.flush();
    client.println(page);
    client.println();
  }
}

// returns if the measured distance of a US Sensor is greater than maxDist
bool isFar(float sensor, float maxDist = 0.3) {
  if (sensor == -1) return true;
  if (sensor > maxDist) return true;
  return false;
}

void handleAutoDrive() {
  if(!autoDriveActive) return;
  // Right US Sensor
  float us1 = measureDistance(US1_PIN);
  // Middle US Sensor
  float us2 = measureDistance(US2_PIN);
  // Left US Sensor
  float us3 = measureDistance(US3_PIN);
  // No obstical detected
  if (isFar(us1) && isFar(us2) && isFar(us3)) {
    drive(true, 50, DEFAULT_SPEED);
    return;
  }
  if (us1 > us2) {
    //turning left
    turn(true, 50, DEFAULT_SPEED);
    return;
  } else {
    // turn right
    turn(false, 50, DEFAULT_SPEED);
    return;
  }
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

void turn(bool left, uint16_t time, uint16_t speed) {
  setMotor(true, !left, speed);
  setMotor(false, left, speed);
  delay(time);

  stopMotor(true);
  stopMotor(false);
}

void drive(bool forward, uint16_t time, uint16_t speed) {
  setMotor(forward, true, speed);
  setMotor(forward, false, speed);
  delay(time);

  stopMotor(true);
  stopMotor(false);
}

void setMotor(bool forward, bool motorA, uint16_t speed) {
  uint8_t pin1 = motorA ? MOTOR_A1_PIN : MOTOR_B2_PIN;
  uint8_t pin2 = motorA ? MOTOR_A2_PIN : MOTOR_B1_PIN;

  uint16_t speed_10 = map(speed, 0, 65535, 0, 1023);  // map 16bit to 10bit value

  switch (forward) {
    case true:
      // drive forward
      analogWrite(pin1, LOW);
      analogWrite(pin2, speed_10);
      break;

    case false:
      // drive backward
      analogWrite(pin1, speed_10);
      analogWrite(pin2, LOW);
      break;
  }
}

void stopMotor(bool motorA) {
  uint8_t pin1 = motorA ? MOTOR_A1_PIN : MOTOR_B2_PIN;
  uint8_t pin2 = motorA ? MOTOR_A2_PIN : MOTOR_B1_PIN;
  
  analogWrite(pin1, LOW);
  analogWrite(pin2, LOW);
}