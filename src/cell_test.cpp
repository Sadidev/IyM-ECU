#include "cell_test.h"
#include <Arduino.h>
#include "pins.h"
#include "HX711.h"

void calibrateCell(HX711 scale) {
  if (scale.is_ready()) {
    scale.set_scale();    
    Serial.println("Tare... remove any weights from the scale.");
    delay(5000);
    scale.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(5000);
    long reading = scale.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
  } 
  else {
    Serial.println("HX711 not found.");
  }
  delay(1000);
}

void testCell(HX711 scale) {
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10), 5);

  scale.power_down(); // put the ADC in sleep mode
  delay(5000);
  scale.power_up();
}