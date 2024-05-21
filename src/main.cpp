#include <Arduino.h>
#include "pins.h"
#include "accelerator.h"
// tests
#include "pot_test.h"


#define TEST_MODE 0

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize pins
  pinMode(ACCELERATOR_PIN, INPUT);
  // pinMode(BREAK_PIN, INPUT);
  // pinMode(TEMPERATURE_PIN, INPUT);
  pinMode(SEATBELT_PIN, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(ENCODER_PIN, INPUT);
}

void loop() {
  if (TEST_MODE) {
    testPot();

    return;
  }
  float motorSpeed = analogRead(ENCODER_PIN);

  bool isSeatbeltOn = digitalRead(SEATBELT_PIN);

  if (!isSeatbeltOn && motorSpeed < 0.1) {
    return;
  }

  if (!isSeatbeltOn && motorSpeed > 0.1) {
    //displayAlarm();
    //sendAlarm();
  }

  float acceleratorValue = getAcceleratorValue8Bits();

  analogWrite(MOTOR_PIN, acceleratorValue);
}