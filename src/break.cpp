#include "break.h"
#include <Arduino.h>
#include "pins.h"
#include "HX711.h"

float getBreakValuePercentage(HX711 chargeCell) {
    // Break can support up to 1Kg 
    const int maxValue = 1000;
    long reading = chargeCell.get_units(10);

    float breakValue = reading/maxValue;

    return breakValue;
}