#ifndef RELAY_MANAGER_H
#define RELAY_MANAGER_H

#include "Config.h"

bool heaterState = false;

void setupRelays() {
  pinMode(RELAY_HEATER_1_GPIO, OUTPUT);
  pinMode(RELAY_HEATER_2_GPIO, OUTPUT);
  pinMode(RELAY_MIST_1_GPIO, OUTPUT);
  pinMode(RELAY_MIST_2_GPIO, OUTPUT);

  digitalWrite(RELAY_HEATER_1_GPIO, HIGH);
  digitalWrite(RELAY_HEATER_2_GPIO, HIGH);
  digitalWrite(RELAY_MIST_1_GPIO, HIGH);
  digitalWrite(RELAY_MIST_2_GPIO, HIGH);
}

/* ===== HEATER ===== */
void heaterON() {
  digitalWrite(RELAY_HEATER_1_GPIO, LOW);
  digitalWrite(RELAY_HEATER_2_GPIO, LOW);
  heaterState = true;
}

void heaterOFF() {
  digitalWrite(RELAY_HEATER_1_GPIO, HIGH);
  digitalWrite(RELAY_HEATER_2_GPIO, HIGH);
  heaterState = false;
}

void temperatureControl(float temp, bool sensorOK) {
  if (!sensorOK) {
    heaterOFF();
    return;
  }

  if (!heaterState && temp < (TEMPERATURE_MIN_C - TEMPERATURE_HYSTERESIS_C)) {
    heaterON();
  }

  if (heaterState && temp > (TEMPERATURE_MAX_C + TEMPERATURE_HYSTERESIS_C)) {
    heaterOFF();
  }
}

/* ===== MIST (SUMMER MODE) ===== */
void mistON() {
  digitalWrite(RELAY_MIST_1_GPIO, LOW);
}

void mistOFF() {
  digitalWrite(RELAY_MIST_1_GPIO, HIGH);
}

void summerMistControl(float temp, float hum, bool sensorOK) {
  if (!sensorOK) {
    mistOFF();
    return;
  }

  if (temp > SUMMER_TEMP_LIMIT_C || hum < SUMMER_HUMIDITY_LIMIT) {
    mistON();
  } else {
    mistOFF();
  }
}

#endif
