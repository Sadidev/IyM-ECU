#include "break.h"
#include <Arduino.h>
#include "pins.h"
#include "HX711.h"

float getBreakValuePercentage(HX711 break) {
    // Break can support up to 1Kg 
    const maxValue = 1000;
    long reading = break.get_units(10);

    float breakValue = reading/maxValue;

    return breakValue;
}