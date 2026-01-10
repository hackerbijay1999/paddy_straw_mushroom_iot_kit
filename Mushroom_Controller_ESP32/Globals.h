#ifndef GLOBALS_H
#define GLOBALS_H
const unsigned long SESSION_TIMEOUT = 30UL * 60UL * 1000UL; // 30 minutes

/* ===== MODES ===== */
enum ControlMode { AUTO_MODE, MANUAL_MODE };
enum UserRole   { ROLE_NONE, ROLE_OPERATOR, ROLE_ADMIN };

/* ===== SYSTEM STATE ===== */
extern ControlMode currentMode;
extern UserRole currentUser;

extern float targetTempC;
extern bool relayEnabled[4];
extern bool emergencyStop;

/* ===== SESSION ===== */
extern bool sessionActive;
extern unsigned long lastActivityTime;
extern const unsigned long SESSION_TIMEOUT;

/* ===== TIMERS ===== */
extern unsigned long nowMillis;
extern unsigned long lastSensorRead;
extern unsigned long lastMQTTPublish;

/* ===== MQTT CONFIG (RUNTIME) ===== */
extern char mqttBroker[64];
extern uint16_t mqttPort;
extern char mqttBaseTopic[64];


#define SENSOR_INTERVAL 5000
#define MQTT_INTERVAL  10000

#endif
