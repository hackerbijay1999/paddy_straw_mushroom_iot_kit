#ifndef OTA_MANAGER_H
#define OTA_MANAGER_H

#include <ArduinoOTA.h>
#include "Globals.h"

void setupOTA() {
  ArduinoOTA.setHostname("mushroom-controller");

  ArduinoOTA.onStart([]() {
    emergencyStop = true;
    allRelaysOFF();
    Serial.println("🛠 OTA Started");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("✅ OTA Finished");
    emergencyStop = false;
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("❌ OTA Error[%u]\n", error);
    emergencyStop = false;
  });

  ArduinoOTA.begin();
  Serial.println("📡 OTA Ready");
}

void handleOTA() {
  ArduinoOTA.handle();
}

#endif
