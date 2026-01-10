#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <WiFi.h>
#include "Globals.h"
#include "MQTT_Manager.h"

#define LED_GREEN 32
#define LED_RED   15

unsigned long ledTimer = 0;
bool ledState = false;

void setupLEDs() {
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
}

void handleLEDs() {
  unsigned long now = millis();

  /* ===== EMERGENCY ===== */
  if (emergencyStop) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
    return;
  }

  /* ===== ERROR ===== */
  if (!sensorHealthy || !pzemHealthy) {
    if (now - ledTimer > 300) {
      ledTimer = now;
      ledState = !ledState;
      digitalWrite(LED_RED, ledState);
    }
    digitalWrite(LED_GREEN, LOW);
    return;
  }

  /* ===== WIFI OK ===== */
  if (WiFi.isConnected()) {
    digitalWrite(LED_RED, LOW);

    /* MQTT CONNECTED → BLINK */
    if (mqttClient.connected()) {
      if (now - ledTimer > 700) {
        ledTimer = now;
        ledState = !ledState;
        digitalWrite(LED_GREEN, ledState);
      }
    } 
    /* WIFI ONLY */
    else {
      digitalWrite(LED_GREEN, HIGH);
    }
  } 
  else {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
  }
}

#endif
