#include "encoder.h"
#include <Arduino.h>
#include "pins.h"

volatile int pulseCount = 0;
unsigned long lastTime = 0;
float rpm = 0;

// Parámetros del encoder y motor
const float hallResolution = 11.0; // Resolución de Hall
const float precisionReductionRatio = 74.83;
const float ppr = hallResolution * precisionReductionRatio; // Pulsos por revolución
const float reducerReductionRatio = 1/75.0; // Reducción del reductor

void countPulses() {
    pulseCount++;
}

float getEncoderToRPM() {
    unsigned long currentTime = millis();
  
    if (currentTime - lastTime >= 1000) { // Cada segundo
        int currentPulseCount;
        
        // Entrar en sección crítica
        detachInterrupt(digitalPinToInterrupt(ENCODER_PIN)); // Detener la interrupción para cálculo seguro
        currentPulseCount = pulseCount;
        pulseCount = 0; // Resetear contador
        attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), countPulses, RISING); // Reiniciar la interrupción
        
        // Calcular RPM
        float revolutions = (currentPulseCount / ppr) * reducerReductionRatio * 100;
        rpm = revolutions * 60.0; // Convertir a RPM
        
        // Imprimir resultado
        Serial.print("RPM: ");
        Serial.println(rpm);
        
        // Actualizar el tiempo
        lastTime = currentTime;

        return rpm;
    }
}
