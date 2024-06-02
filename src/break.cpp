#include "break.h"
#include <Arduino.h>
#include "pins.h"
#include "HX711.h"

float getBreakValuePercentage(HX711 chargeCell) {
    // Break can support up to 1Kg 
    const int maxValue = 1000;
    long reading = abs(chargeCell.get_units(10));

    Serial.print("Break average reading: ");
    Serial.println(reading);

    float breakValue = (static_cast<float>(reading) / maxValue) * 100; // Need to convert type to work

    Serial.print("Break Percentage: ");
    Serial.println(breakValue);

    return breakValue;
}