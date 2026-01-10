#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#include <WiFi.h>
#include "RTC_Manager.h"
#include "Globals.h"

/* ================= MQTT CLIENT ================= */
WiFiClient espClient;
PubSubClient mqttClient(espClient);

/* ================= STATE ================= */
unsigned long lastMQTTAttempt = 0;
bool mqttConnected = false;

/* ================= TOPICS ================= */
#define TOPIC_BASE      "mushroom/farm"
#define TOPIC_TELEMETRY TOPIC_BASE "/telemetry"
#define TOPIC_STATUS    TOPIC_BASE "/status"
#define TOPIC_RELAYS    TOPIC_BASE "/relays"
#define TOPIC_EVENTS    TOPIC_BASE "/events"
#define TOPIC_LWT       TOPIC_BASE "/lwt"

/* ================= FORWARD DECL ================= */
void mqttEvent(String event, String data = "{}");

/* ================= SETUP ================= */
void setupMQTT() {
  mqttClient.setServer(mqttBroker, mqttPort);
}

/* ================= NON-BLOCKING HANDLER ================= */
void handleMQTT() {

  if (mqttClient.connected()) {
    mqttClient.loop();
    return;
  }

  if (millis() - lastMQTTAttempt < 5000) return;
  lastMQTTAttempt = millis();

  if (mqttClient.connect(
        "MushroomESP32",
        TOPIC_LWT,
        1,
        true,
        "{\"status\":\"offline\"}"
      )) {

    mqttConnected = true;
    mqttClient.publish(TOPIC_LWT, "{\"status\":\"online\"}", true);
    mqttEvent("MQTT_CONNECTED");
  }
}

/* ================= EVENT LOGGER ================= */
void mqttEvent(String event, String data) {
  if (!mqttClient.connected()) return;

  String payload =
    "{"
    "\"ts\":\"" + getTimestamp() + "\","
    "\"event\":\"" + event + "\","
    "\"data\":" + data +
    "}";

  mqttClient.publish(TOPIC_EVENTS, payload.c_str());
}

/* ================= TELEMETRY ================= */
void publishTelemetry(float temp, float hum, float volt, float power) {
  if (!mqttClient.connected()) return;

  String payload =
    "{"
    "\"ts\":\"" + getTimestamp() + "\","
    "\"temp\":" + String(temp) + ","
    "\"hum\":" + String(hum) + ","
    "\"voltage\":" + String(volt) + ","
    "\"power\":" + String(power) +
    "}";

  mqttClient.publish(TOPIC_TELEMETRY, payload.c_str());
}

/* ================= STATUS ================= */
void publishStatus() {
  if (!mqttClient.connected()) return;

  String mode =
    emergencyStop ? "EMERGENCY" :
    (currentMode == AUTO_MODE ? "AUTO" : "MANUAL");

  String payload =
    "{"
    "\"ts\":\"" + getTimestamp() + "\","
    "\"mode\":\"" + mode + "\","
    "\"uptime\":" + String(millis() / 1000) +
    "}";

  mqttClient.publish(TOPIC_STATUS, payload.c_str());
}

#endif
