#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <Preferences.h>
#include "Globals.h"
#include "Relay_Manager.h"

/* ================= VERSION ================= */
#define CONFIG_VERSION 2
char mqttBroker[64];
uint16_t mqttPort;
char mqttBaseTopic[64];

Preferences prefs;

/* ================= LOAD CONFIG ================= */
void loadConfig() {
  prefs.begin("mushroom", false);

  uint8_t ver = prefs.getUChar("ver", 0);
  if (ver != CONFIG_VERSION) {
    prefs.clear();
    prefs.putUChar("ver", CONFIG_VERSION);
  }

  targetTempC   = prefs.getFloat("tset", 31.0);
  currentMode   = (ControlMode)prefs.getUChar("mode", AUTO_MODE);
  emergencyStop = prefs.getBool("emg", false);

  for (int i = 0; i < 4; i++) {
    relayEnabled[i] = prefs.getBool(("re" + String(i)).c_str(), true);
  }

  // ✅ MQTT CONFIG
  strlcpy(mqttBroker,
          prefs.getString("mb", MQTT_DEFAULT_BROKER).c_str(),
          sizeof(mqttBroker));

  mqttPort = prefs.getUInt("mp", MQTT_DEFAULT_PORT);

  strlcpy(mqttBaseTopic,
          prefs.getString("mt", MQTT_DEFAULT_BASE_TOPIC).c_str(),
          sizeof(mqttBaseTopic));

  prefs.end();

  Serial.println("✅ Config loaded");
  Serial.println("MQTT Broker: " + String(mqttBroker));
  Serial.println("MQTT Port: " + String(mqttPort));
  Serial.println("MQTT Topic: " + String(mqttBaseTopic));
}

/* ================= SAVE CONFIG ================= */
void saveConfig() {
  prefs.begin("mushroom", false);

  prefs.putUChar("ver", CONFIG_VERSION);
  prefs.putFloat("tset", targetTempC);
  prefs.putUChar("mode", currentMode);
  prefs.putBool("emg", emergencyStop);
  prefs.putString("mb", mqttBroker);
  prefs.putUInt("mp", mqttPort);
  prefs.putString("mt", mqttBaseTopic);


  for (int i = 0; i < 4; i++) {
    prefs.putBool(("re" + String(i)).c_str(), relayEnabled[i]);
  }

  prefs.end();
}

/* ================= RELAY STATE ================= */
void saveRelayStates() {
  prefs.begin("mushroom", false);

  for (int i = 0; i < 4; i++) {
    bool state = digitalRead(relayPins[i]) == LOW;
    prefs.putBool(("rs" + String(i)).c_str(), state);
  }

  prefs.end();
}

void restoreRelayStates() {
  prefs.begin("mushroom", false);

  for (int i = 0; i < 4; i++) {
    bool state = prefs.getBool(("rs" + String(i)).c_str(), false);
    digitalWrite(relayPins[i], state ? LOW : HIGH);
  }

  prefs.end();
}

#endif
