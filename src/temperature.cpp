#include "temperature.h"
#include <Arduino.h>
#include "pins.h"

#define TEMP_MAX_VALUE_CELSIUS 70.0

float getTemperatureValue() {
    float initialValue = analogRead(TEMPERATURE_PIN);
    float convertedValue = initialValue * (TEMP_MAX_VALUE_CELSIUS/3102.0); // Usamos este valor y no 4095 ya que el amplificador envia 2.5 para tener un margen de seguridad
    
    Serial.print("Temp: ");
    Serial.println(convertedValue);

    return convertedValue;
}

uint32_t processAcceleratorValue(float acceleratorValue, float temperatureValue) {
    volatile float processedValue = acceleratorValue;
    const float maxPWMValue = 255.0;
   
    if (temperatureValue >= 20 && temperatureValue < 35) {
        processedValue = (processedValue * 1.2);
    } 

    if (temperatureValue >= 35 && temperatureValue < 50) {
        processedValue = (processedValue * 1.1);
    }

    float finalProcessedValue = min(static_cast<float>(processedValue), static_cast<float>(maxPWMValue)); // Cast both values as float
    
    Serial.print("FinalProcessedValue: ");
    Serial.println(finalProcessedValue);

    return (uint32_t)finalProcessedValue;
}