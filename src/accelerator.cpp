#include "accelerator.h"
#include <Arduino.h>
#include "pins.h"

float getAcceleratorValue8Bits() {
    float initialValue = analogRead(ACCELERATOR_PIN);
    float convertedValue = initialValue * (255.0/1023.0);

    return convertedValue;
}
