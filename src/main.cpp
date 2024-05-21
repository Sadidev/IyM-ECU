#include <Arduino.h>
#include <ArduinoJson.h>
#include "pins.h"
#include "connection.h"
#include "accelerator.h"
// tests
#include "pot_test.h"


#define TEST_MODE 0

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  setupWifi();
  setupMQTTConnection();

  // Initialize pins
  pinMode(ACCELERATOR_PIN, INPUT);
  // pinMode(BREAK_PIN, INPUT);
  // pinMode(TEMPERATURE_PIN, INPUT);
  // pinMode(SEATBELT_PIN, INPUT);
  // pinMode(MOTOR_PIN, OUTPUT);
  // pinMode(ENCODER_PIN, INPUT);
}

void loop() {
  if (TEST_MODE) {
    testPot();

    return;
  }

  checkMQTTConnection();

  // float motorSpeed = analogRead(ENCODER_PIN);

  // bool isSeatbeltOn = digitalRead(SEATBELT_PIN);

  // if (!isSeatbeltOn && motorSpeed < 0.1) {
  //   return;
  // }

  // if (!isSeatbeltOn && motorSpeed > 0.1) {
  //   //displayAlarm();
  //   //sendAlarm();
  // }

  float acceleratorValue = getAcceleratorValue8Bits();
  DynamicJsonDocument data(256);
  data["pot"] = acceleratorValue;

  publishTelemetry(data);
  // analogWrite(MOTOR_PIN, acceleratorValue);
  delay(1000);
}