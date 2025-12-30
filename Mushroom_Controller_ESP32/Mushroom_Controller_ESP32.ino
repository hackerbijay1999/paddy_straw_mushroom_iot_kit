#include "Config.h"
#include "WiFi_Manager.h"
#include "DHT_Manager.h"
#include "Relay_Manager.h"
#include "PID_Manager.h"
#include "Blynk_Manager.h"
#include "MQTT_Manager.h"

void setup() {
  Serial.begin(115200);

  setupRelays();
  setupDHTSensors();
  setupWiFi();
  setupBlynk();
  setupMQTT();
  setupPID();
}

void loop() {
  handleWiFiReconnect();
  runBlynk();

  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  readDHTSensors();

  if (!sensorHealthy) {
    heaterOFF();
    mistOFF();
    return;
  }

  /* PID heater control */
  if (pidHeaterDecision(avgTempC)) heaterON();
  else heaterOFF();

  /* Summer mist control */
  summerMistControl(avgTempC, avgHumidity, sensorHealthy);

  updateBlynk(avgTempC, avgHumidity);
  publishMQTT(avgTempC, avgHumidity);

  delay(5000);
}
