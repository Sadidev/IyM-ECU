#include <Arduino.h>
#include <ArduinoJson.h>
#include "soc/rtc.h"
#include "HX711.h"
// our libs
#include "pins.h"
#include "connection.h"
#include "accelerator.h"
// tests
#include "pot_test.h"
#include "cell_test.h"


#define TEST_MODE 1

HX711 scale;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  setupWifi();
  setupMQTTConnection();

  // Initialize pins
  pinMode(ACCELERATOR_PIN, INPUT);
  // pinMode(TEMPERATURE_PIN, INPUT);
  // pinMode(SEATBELT_PIN, INPUT);
  // pinMode(MOTOR_PIN, OUTPUT);
  // pinMode(ENCODER_PIN, INPUT);

  // Initialize Data bus config for HX711 module
  rtc_cpu_freq_config_t config;
  rtc_clk_cpu_freq_get_config(&config);
  rtc_clk_cpu_freq_to_config(RTC_CPU_FREQ_80M, &config);
  rtc_clk_cpu_freq_set_config_fast(&config);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {
  if (TEST_MODE) {
    testPot();
    testCell(scale);
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