#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#ifndef ENV_VARIABLES
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define TB_MQTT_SERVER ""
#define TB_MQTT_PORT 0
#define TB_DEVICE_TOKEN ""
#endif

#define TELEMETRY_ROUTE "v1/devices/me/telemetry"

WiFiClient espClient;
PubSubClient client(espClient);

void setupWifi() {
    delay(10);
    Serial.print("Connecting to: ");
    Serial.println(WIFI_SSID);

    WiFi.mode(WIFI_STA); // Declare the ESP as STATION
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("Connected!");
    Serial.println("Ip address assigned: ");
    Serial.println(WiFi.localIP());
}

void setupMQTTConnection() {
    client.setServer(TB_MQTT_SERVER, TB_MQTT_PORT);
    Serial.println("Server set: ");
    Serial.print(TB_MQTT_SERVER);
    // Set Buffer size to be larger to sustain sedning network json data
    client.setBufferSize(2048);
}

void checkMQTTConnection() {
    while (!client.connected()) {
        Serial.println("Trying to connect MQTT...");

        if (client.connect("ESP32", TB_DEVICE_TOKEN, TB_DEVICE_TOKEN)) {
            Serial.println("Connected!");
        } else {
            Serial.println("Error, rc = ");
            Serial.print(client.state());
        }
    }
}

void publishTelemetry(DynamicJsonDocument data) {
    if (!client.connected()) {
        Serial.println("Unable to publish message");
        return;
    }

    char buffer[256];
    serializeJson(data, buffer);
    client.publish(TELEMETRY_ROUTE, buffer);

    Serial.println("Publish message [telemetry]: ");
    Serial.println(buffer);
}
