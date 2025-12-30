#include "Config.h"
#include "WiFi_Manager.h"
#include "DHT_Manager.h"
#include "Relay_Manager.h"
#include "PID_Manager.h"
#include "Blynk_Manager.h"
#include "MQTT_Manager.h"
#include "OTA_Manager.h"
#include "Alert_Manager.h"


void setup() {
  Serial.begin(115200);

  setupRelays();
  setupDHTSensors();
  setupWiFi();
  setupBlynk();
  setupMQTT();
  setupPID();
  setupOTA();
  setupAlerts();
  sendAlert("⚡ Power restored. Mushroom controller is back online.");



}

void loop() {
  handleWiFiReconnect();
  runBlynk();
  handleOTA();

  /* ===== WIFI ALERT ===== */
  if (WiFi.status() != WL_CONNECTED) {
    if (!wifiOfflineAlertSent) {
      sendAlert("📡 Wi-Fi disconnected! System running offline.");
      wifiOfflineAlertSent = true;
    }
  } else {
    wifiOfflineAlertSent = false;
  }

  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  readDHTSensors();

  /* ===== SENSOR FAILURE ===== */
  if (!sensorHealthy) {
    heaterOFF();
    mistOFF();
    sendAlert("⚠ DHT Sensor Failure! System halted.");
    return;
  }

  /* ===== TEMP ALERT ===== */
  if (avgTempC > 38.0) {
    sendAlert("🔥 Temperature too high in mushroom room!");
  }

  /* ===== HUMIDITY ALERT ===== */
  if (avgHumidity < 60) {
    sendAlert("💧 Humidity too LOW for mushrooms!");
  }
  if (avgHumidity > 95) {
    sendAlert("⚠ Humidity too HIGH – contamination risk!");
  }

  /* ===== PID HEATER CONTROL + STUCK DETECT ===== */
  if (pidHeaterDecision(avgTempC)) {
    heaterON();
    if (heaterOnStartTime == 0) heaterOnStartTime = millis();
    if (millis() - heaterOnStartTime > HEATER_MAX_ON_TIME && !heaterStuckAlertSent) {
      sendAlert("🔥 Heater ON too long! Possible relay/SSR failure.");
      heaterStuckAlertSent = true;
    }
  } else {
    heaterOFF();
    heaterOnStartTime = 0;
    heaterStuckAlertSent = false;
  }

  /* ===== SUMMER MIST ===== */
  summerMistControl(avgTempC, avgHumidity, sensorHealthy);

  updateBlynk(avgTempC, avgHumidity);
  publishMQTT(avgTempC, avgHumidity);

  delay(5000);
}
