#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

void setupWiFi() {
  wifiMulti.addAP("HomeWiFi", "password1");
  wifiMulti.addAP("FarmWiFi", "password2");
  wifiMulti.addAP("MobileHotspot", "password3");

  Serial.print("Connecting to WiFi");
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  Serial.println(WiFi.localIP());
}

void handleWiFiReconnect() {
  if (wifiMulti.run() != WL_CONNECTED) {
    setupWiFi();
  }
}

#endif
