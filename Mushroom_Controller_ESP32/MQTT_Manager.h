#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#include "Config.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setupMQTT() {
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    mqttClient.connect("MushroomESP32");
    delay(1000);
  }
}

void publishMQTT(float temp, float hum) {
  char payload[100];
  snprintf(payload, sizeof(payload),
           "{\"temperature\":%.2f,\"humidity\":%.2f}",
           temp, hum);

  mqttClient.publish(MQTT_TOPIC, payload);
}

#endif
