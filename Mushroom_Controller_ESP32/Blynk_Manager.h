#ifndef BLYNK_MANAGER_H
#define BLYNK_MANAGER_H

#include "Config.h"
#include "Globals.h"
#include "Alert_Manager.h"
#include "Relay_Manager.h"
#include <BlynkSimpleEsp32.h>

/* ===== SETUP ===== */
void setupBlynk() {
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();
}

/* ===== RUN ===== */
void runBlynk() {
  Blynk.run();
}

/* ===== DASHBOARD ===== */
void updateBlynk(float temperature, float humidity) {
  Blynk.virtualWrite(VPIN_AVG_TEMPERATURE, temperature);
  Blynk.virtualWrite(VPIN_AVG_HUMIDITY, humidity);
}

/* ===== EMERGENCY STOP BUTTON ===== */
BLYNK_WRITE(V10) {   // Button widget → V10
  emergencyStop = param.asInt();

  if (emergencyStop) {
    allRelaysOFF();
    Serial.println("🚨 EMERGENCY STOP (BLYNK)");
    sendAlert("🚨 Emergency STOP activated from Blynk!");
  } else {
    Serial.println("✅ Emergency STOP released");
  }
}

#endif
