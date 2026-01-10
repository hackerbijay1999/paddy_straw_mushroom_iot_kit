/************************************************************
 *  INCLUDES
 ************************************************************/
#include "Globals.h"
#include "Config.h"
#include "WiFi_Manager.h"
#include "DHT_Manager.h"
#include "Relay_Manager.h"
#include "Blynk_Manager.h"
#include "MQTT_Manager.h"
#include "OTA_Manager.h"
#include "Alert_Manager.h"
#include "Storage_Manager.h"
#include "Web_Config.h"
#include "RTC_Manager.h"
#include "PZEM_Manager.h"
#include "Button_Manager.h"
#include "LED_Manager.h"

/************************************************************
 *  GLOBAL DEFINITIONS (ONLY DECLARATIONS)
 ************************************************************/
/******** GLOBAL DEFINITIONS (ONLY PLACE) ********/
ControlMode currentMode = AUTO_MODE;
UserRole currentUser = ROLE_NONE;

float targetTempC = 31.0;
bool relayEnabled[4] = { true, true, true, true };
bool emergencyStop = false;

bool sessionActive = false;
unsigned long lastActivityTime = 0;


unsigned long nowMillis = 0;
unsigned long lastSensorRead = 0;
unsigned long lastMQTTPublish = 0;

/************************************************************
 *  SETUP
 ************************************************************/
void setup() {
  Serial.begin(115200);
  delay(300);

  setupRelays();       // 1️⃣ GPIO ready
  loadConfig();        // 2️⃣ load NVS
  restoreRelayStates();// 3️⃣ restore outputs

  setupPZEM();
  setupDHTSensors();
  setupWiFi();
  setupRTC();
  setupButtons();

  setupBlynk();
  setupMQTT();
  setupOTA();
  setupAlerts();
  setupWebServer();

  Serial.println("⏰ RTC Time:");
  Serial.println(getTimestamp());
}

/************************************************************
 *  AUTO CONTROL LOGIC
 ************************************************************/
void runAutoControl() {
  if (avgTempC < targetTempC - TEMPERATURE_HYSTERESIS_C) {
    setRelay(0, true);
  } else if (avgTempC > targetTempC + TEMPERATURE_HYSTERESIS_C) {
    setRelay(0, false);
  }

  if (avgTempC > SUMMER_TEMP_LIMIT_C || avgHumidity < SUMMER_HUMIDITY_LIMIT) {
    setRelay(2, true);
  } else {
    setRelay(2, false);
  }
}

/************************************************************
 *  LOOP (NON-BLOCKING)
 ************************************************************/
void loop() {
  nowMillis = millis();

  runBlynk();
  handleOTA();
  handleWebServer();
  handleButtons();
  handleMQTT();
  handleLEDs();

  /* ===== EMERGENCY STOP ===== */
  if (emergencyStop) {
    allRelaysOFF();
    return;
  }

  /* ===== SESSION TIMEOUT ===== */
  if (sessionActive && millis() - lastActivityTime > SESSION_TIMEOUT) {
  Serial.println("🔐 Web session timed out");
}


  /* ===== SENSOR READ ===== */
  if (nowMillis - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = nowMillis;
    readDHTSensors();
    readPZEM();
  }

  /* ===== SAFETY ===== */
  if (!sensorHealthy) {
    allRelaysOFF();
    sendAlert("⚠ Sensor failure! System halted.");
    return;
  }

  if (!pzemHealthy) {
    sendAlert("⚠ Power meter communication failure!");
  }

  /* ===== CONTROL ===== */
  if (currentMode == AUTO_MODE) {
    runAutoControl();
  }

  /* ===== MQTT PUBLISH ===== */
  if (nowMillis - lastMQTTPublish >= MQTT_INTERVAL) {
    lastMQTTPublish = nowMillis;

    publishTelemetry(avgTempC, avgHumidity, voltage, power);
    publishStatus();
    updateBlynk(avgTempC, avgHumidity);

    mqttEvent("SENSOR_READ");
  }
}
