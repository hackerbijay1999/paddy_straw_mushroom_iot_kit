#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFiManager.h>
#include <ESPmDNS.h>   // ✅ REQUIRED FOR MDNS

WiFiManager wm;

bool shouldSaveConfig = false;

void saveConfigCallback() {
  shouldSaveConfig = true;
}

void setupWiFi() {
  wm.setSaveConfigCallback(saveConfigCallback);
  wm.setConfigPortalTimeout(180);

  if (!wm.autoConnect("Mushroom_Controller_AP")) {
    ESP.restart();
  }

  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());
  sessionActive = false;
  currentUser = ROLE_NONE;
  lastActivityTime = 0;

  // ✅ mDNS (optional feature)
  if (MDNS.begin("mushroom")) {
    Serial.println("mDNS started: http://mushroom.local");
  } else {
    Serial.println("mDNS failed");
  }
}

void resetWiFi() {
  wm.resetSettings();
  ESP.restart();
}

#endif
