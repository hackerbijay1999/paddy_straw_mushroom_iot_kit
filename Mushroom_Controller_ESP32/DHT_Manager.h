#ifndef DHT_MANAGER_H
#define DHT_MANAGER_H

#include <DHT.h>
#include "Config.h"

/* ===== DHT OBJECTS ===== */
DHT dhtRoom(DHT_ROOM_SENSOR_GPIO, DHT_SENSOR_TYPE);
DHT dhtBed(DHT_MUSHROOM_BED_GPIO, DHT_SENSOR_TYPE);

/* ===== READINGS ===== */
float roomTempC = 0, roomHumidity = 0;
float bedTempC  = 0, bedHumidity  = 0;
float avgTempC  = 0, avgHumidity  = 0;

bool sensorHealthy = true;

/* ===== TIMING ===== */
unsigned long lastDHTRead = 0;
#define DHT_INTERVAL  3000   // 3 seconds

/* ===== SETUP ===== */
void setupDHTSensors() {
  dhtRoom.begin();
  dhtBed.begin();
}

/* ===== READ FUNCTION ===== */
void readDHTSensors() {

  if (millis() - lastDHTRead < DHT_INTERVAL) return;
  lastDHTRead = millis();

  float rt = dhtRoom.readTemperature();
  float rh = dhtRoom.readHumidity();

  float bt = dhtBed.readTemperature();
  float bh = dhtBed.readHumidity();

  /* ===== VALIDATION ===== */
  if (isnan(rt) || isnan(bt) || isnan(rh) || isnan(bh)) {
    sensorHealthy = false;
    Serial.println("❌ DHT read failed");
    return;
  }

  /* ===== ASSIGN ===== */
  roomTempC = rt;
  roomHumidity = rh;
  bedTempC = bt;
  bedHumidity = bh;

  avgTempC = (roomTempC + bedTempC) / 2.0;
  avgHumidity = (roomHumidity + bedHumidity) / 2.0;

  sensorHealthy = true;

  /* ===== DEBUG ===== */
  Serial.print("🌡 Room: ");
  Serial.print(roomTempC);
  Serial.print(" °C | Bed: ");
  Serial.print(bedTempC);
  Serial.print(" °C | Avg: ");
  Serial.print(avgTempC);

  Serial.print(" 💧 Avg RH: ");
  Serial.println(avgHumidity);
}

#endif
