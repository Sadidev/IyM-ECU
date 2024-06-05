#include "temperature.h"
#include <Arduino.h>
#include "pins.h"

float getTemperatureValue8Bits() {
    float initialValue = analogRead(TEMPERATURE_PIN);
    float convertedValue = initialValue * (255.0/4095.0);
    
    Serial.print("Temp: ");
    Serial.println(convertedValue);

    return convertedValue;
}
