#include "temperature.h"
#include <Arduino.h>
#include "pins.h"

#define TEMP_MAX_VALUE_CELSIUS 70.0

float getTemperatureValue() {
    float initialValue = analogRead(TEMPERATURE_PIN);
    float convertedValue = initialValue * (TEMP_MAX_VALUE_CELSIUS/4095.0);
    
    Serial.print("Temp: ");
    Serial.println(convertedValue);

    return convertedValue;
}

uint32_t processAcceleratorValue(float acceleratorValue, float temperatureValue) {
    volatile float processedValue = acceleratorValue;
   
    if (temperatureValue >= 20 && temperatureValue < 35) {
        processedValue = (processedValue * 1.2);
    } 

    if (temperatureValue >= 35 && temperatureValue < 50) {
        processedValue = (processedValue * 1.1);
    }

    return (uint32_t)processedValue;
}