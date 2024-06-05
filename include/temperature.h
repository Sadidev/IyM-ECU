#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include <Arduino.h>

float getTemperatureValue();

uint32_t processAcceleratorValue(float acceleratorValue, float temperatureValue);

#endif // TEMPERATURE_H