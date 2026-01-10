
#ifndef CONFIG_H
#define CONFIG_H

#define WEB_USERNAME "admin"
#define WEB_PASSWORD "admin123"

#define WEB_ADMIN_USER     "admin"
#define WEB_ADMIN_PASS     "admin123"

#define WEB_OPERATOR_USER  "operator"
#define WEB_OPERATOR_PASS  "1234"

/* ========== BLYNK IOT (MANDATORY) ========== */
#define BLYNK_TEMPLATE_ID "TMPL3dwHpndiO"
#define BLYNK_TEMPLATE_NAME "Mushroom Farm Controller"
#define BLYNK_AUTH_TOKEN "yNrD7eZw3Ejc5eIn7M4ZKBuA_fve2GLo"

#define BLYNK_PRINT Serial

/* ===== DHT CONFIGURATION ===== */
#define DHT_SENSOR_TYPE               DHT22
#define DHT_ROOM_SENSOR_GPIO          4
#define DHT_MUSHROOM_BED_GPIO         5

/* ===== RELAY GPIO ===== */
#define RELAY_HEATER_1_GPIO           14
#define RELAY_HEATER_2_GPIO           27
#define RELAY_MIST_1_GPIO             26   // future
#define RELAY_MIST_2_GPIO             25   // future

/* ===== TEMPERATURE LIMITS ===== */
#define TEMPERATURE_MIN_C             28.0
#define TEMPERATURE_MAX_C             35.0

/* ===== BLYNK VIRTUAL PINS ===== */
#define VPIN_AVG_TEMPERATURE          V1
#define VPIN_AVG_HUMIDITY             V2

/* ===== MQTT ===== */
#define MQTT_DEFAULT_BROKER     "broker.hivemq.com"
#define MQTT_DEFAULT_PORT       1883
#define MQTT_DEFAULT_BASE_TOPIC "mushroom"
#define MQTT_CLIENTID "MushroomESP32"


/* ===== SAFETY & CONTROL ===== */
#define TEMPERATURE_HYSTERESIS_C     1.0

/* ===== SUMMER MODE ===== */
#define SUMMER_TEMP_LIMIT_C          34.0
#define SUMMER_HUMIDITY_LIMIT        85.0

/* ===== PID SETPOINT ===== */
//#define PID_TARGET_TEMP_C            31.0

/* ===== TELEGRAM ALERT ===== */
#define TELEGRAM_BOT_TOKEN   "8356866552:AAEH_7BUZSyY0IahLF9C15-iUuSR47HMP0w"
#define TELEGRAM_CHAT_ID    "123456789"



#endif
