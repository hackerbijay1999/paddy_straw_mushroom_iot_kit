#ifndef RELAY_MANAGER_H
#define RELAY_MANAGER_H

#include "Config.h"

/* ===== EXTERNALS ===== */
extern bool relayEnabled[4];

/* ===== FORWARD DECL ===== */
void saveRelayStates();   // from Storage_Manager

/* ===== RELAY GPIO MAP ===== */
uint8_t relayPins[4] = {
  RELAY_HEATER_1_GPIO,
  RELAY_HEATER_2_GPIO,
  RELAY_MIST_1_GPIO,
  RELAY_MIST_2_GPIO
};

/* ===== SETUP ===== */
void setupRelays() {
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH); // OFF default
  }
}

/* ===== CONTROL ===== */
void setRelay(uint8_t ch, bool state) {
  if (ch > 3) return;
  if (!relayEnabled[ch]) return;

  digitalWrite(relayPins[ch], state ? LOW : HIGH);
  saveRelayStates();   // persist only
}

/* ===== ALL OFF ===== */
void allRelaysOFF() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(relayPins[i], HIGH);
  }
  saveRelayStates();
}

#endif
