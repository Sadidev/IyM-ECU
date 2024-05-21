#ifndef CONNECTION_H
#define CONNECTION_H
#include <ArduinoJson.h>

void setupWifi();

void setupMQTTConnection();

void checkMQTTConnection();

void publishTelemetry(DynamicJsonDocument data);

#endif // CONNECTION_H