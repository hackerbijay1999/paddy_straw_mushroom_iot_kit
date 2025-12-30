#ifndef BLYNK_MANAGER_H
#define BLYNK_MANAGER_H

#include "Config.h"
#include <BlynkSimpleEsp32.h>

/* Initialize Blynk (uses existing WiFi connection) */
void setupBlynk() {
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();
}

/* Run Blynk */
void runBlynk() {
  Blynk.run();
}

/* Update dashboard */
void updateBlynk(float temperature, float humidity) {
  Blynk.virtualWrite(VPIN_AVG_TEMPERATURE, temperature);
  Blynk.virtualWrite(VPIN_AVG_HUMIDITY, humidity);
}

#endif
