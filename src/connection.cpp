#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define TB_MQTT_SERVER ""
#define TB_MQTT_PORT 0
#define TB_DEVICE_TOKEN ""

WiFiClient espClient;
PubSubClient client(espClient);

void setupWifi() {
    Serial.print("Connecting to: ");
    Serial.print(WIFI_SSID);

    WiFi.mode(WIFI_STA); // Declare the ESP as STATION
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected!");
    Serial.println("Ip address assigned: ");
    Serial.println(WiFi.localIP());
}

void setupMQTTConnection() {
    client.setServer(TB_MQTT_SERVER, TB_MQTT_PORT);
    // Set Buffer size to be larger to sustain sedning network json data
    client.setBufferSize(2048);
}

void checkMQTTConnection() {
    if (!client.connected()) {
        Serial.print("Trying to connect MQTT...");

        if (client.connect("ESP32", TB_DEVICE_TOKEN, TB_DEVICE_TOKEN)) {
            Serial.println("Connected!");
        } else {
            Serial.println("Error, rc = ");
            Serial.print(client.state());
        }
    }
}
