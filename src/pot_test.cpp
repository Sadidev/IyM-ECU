#include "pot_test.h"
#include <Arduino.h>
#include "pins.h"

void testPot() {
    int potValue = analogRead(ACCELERATOR_PIN);

    Serial.println(potValue);
}
