#ifndef DHT_MANAGER_H
#define DHT_MANAGER_H

#include <DHT.h>
#include "Config.h"

DHT dhtRoom(DHT_ROOM_SENSOR_GPIO, DHT_SENSOR_TYPE);
DHT dhtBed(DHT_MUSHROOM_BED_GPIO, DHT_SENSOR_TYPE);

float roomTempC, roomHumidity;
float bedTempC, bedHumidity;
float avgTempC, avgHumidity;

bool sensorHealthy = true;

void setupDHTSensors() {
  dhtRoom.begin();
  dhtBed.begin();
}

void readDHTSensors() {
  roomTempC = dhtRoom.readTemperature();
  roomHumidity = dhtRoom.readHumidity();
  bedTempC = dhtBed.readTemperature();
  bedHumidity = dhtBed.readHumidity();

  if (isnan(roomTempC) || isnan(bedTempC)) {
    sensorHealthy = false;
    return;
  }

  sensorHealthy = true;
  avgTempC = (roomTempC + bedTempC) / 2.0;
  avgHumidity = (roomHumidity + bedHumidity) / 2.0;
}

#endif
