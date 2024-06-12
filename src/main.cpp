#include <Arduino.h>
#include <ArduinoJson.h>
#include "soc/rtc.h"
#include "HX711.h"
// our libs
#include "pins.h"
#include "connection.h"
#include "accelerator.h"
#include "break.h"
#include "encoder.h"
#include "temperature.h"
// tests
#include "pot_test.h"
#include "cell_test.h"


#define TEST_MODE 0
#define CALIBRATION_FACTOR 8483/10.4

HX711 scale;

unsigned long previousPostTimestamp = 0;
const unsigned long postInterval = 10000; // 10000 milliseconds = 10 seconds

const int freq = 5000; 
const int motorChannel = 0; 
const int breakChannel = 1;
const int resolution = 8;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  setupWifi();
  setupMQTTConnection();

  // Initialize pins
  pinMode(ACCELERATOR_PIN, INPUT);
  pinMode(TEMPERATURE_PIN, INPUT);
  pinMode(SEATBELT_PIN, INPUT_PULLUP);

  ledcSetup(motorChannel, freq, resolution);
  ledcAttachPin(MOTOR_PIN, motorChannel);

  ledcSetup(breakChannel, freq, resolution);
  ledcAttachPin(BREAK_PIN, breakChannel);

  pinMode(ENCODER_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), countPulses, RISING);

  // Initialize Data bus config for HX711 module
  rtc_cpu_freq_config_t config;
  rtc_clk_cpu_freq_get_config(&config);
  rtc_clk_cpu_freq_to_config(RTC_CPU_FREQ_80M, &config);
  rtc_clk_cpu_freq_set_config_fast(&config);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read()); // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20)); // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1); // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)
            
  scale.set_scale(CALIBRATION_FACTOR);
  scale.tare(); // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read()); // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20)); // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1); // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale

  previousPostTimestamp = millis();
}

void loop() {
  if (TEST_MODE) {
    testPot();
    testCell(scale);
    return;
  }
  unsigned long currentTimestamp = millis();
  checkMQTTConnection();

  float motorSpeed = getEncoderToRPM();

  int isSeatbeltOn = digitalRead(SEATBELT_PIN);
  Serial.print("isSeatbeltOn: ");
  Serial.println(isSeatbeltOn);


  // if (!isSeatbeltOn && motorSpeed > 0.1) {
  //   //displayAlarm();
  //   sendAlarm();
  // }

  float acceleratorValue = getAcceleratorValue8Bits();
  float breakValuePercentage = getBreakValuePercentage(scale);
  float temperatureValue = getTemperatureValue();

  DynamicJsonDocument data(256);
  data["speed"] = motorSpeed;
  data["seatbelt_status"] = isSeatbeltOn;
  data["temp"] = temperatureValue;

  publishTelemetry(data);

  if (isSeatbeltOn == LOW && motorSpeed < 0.1) {
    Serial.println("Esta Low");
    return;
  }

  // We consider threshold value for break being pushed around 100
  if (breakValuePercentage > 10) {
    uint32_t breakValue8Bits = (u_int32_t)(breakValuePercentage * 255);

    ledcWrite(motorChannel, 0);
    ledcWrite(breakChannel, breakValue8Bits);
    return;
  }
  
  uint32_t dutyCycle =  processAcceleratorValue(acceleratorValue, temperatureValue); // Convert to uint32_t
  ledcWrite(motorChannel, dutyCycle);
  ledcWrite(breakChannel, 0);
}