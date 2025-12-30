#ifndef ALERT_MANAGER_H
#define ALERT_MANAGER_H

#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32.h>
#include "Config.h"

WiFiClientSecure telegramClient;

/* ===== ALERT TIMING ===== */
unsigned long lastAlertTime = 0;
const unsigned long ALERT_INTERVAL = 300000; // 5 minutes

/* ===== ALERT FLAGS ===== */
bool heaterStuckAlertSent = false;
bool wifiOfflineAlertSent = false;

/* ===== SETUP ===== */
void setupAlerts() {
  telegramClient.setInsecure(); // Skip TLS cert validation
}

/* ===== SEND ALERT (TELEGRAM + BLYNK) ===== */
void sendAlert(String message) {
  if (millis() - lastAlertTime < ALERT_INTERVAL) return;

  // ---- Telegram HTTPS API ----
  String url = "/bot" + String(TELEGRAM_BOT_TOKEN) +
               "/sendMessage?chat_id=" + String(TELEGRAM_CHAT_ID) +
               "&text=" + message;

  if (telegramClient.connect("api.telegram.org", 443)) {
    telegramClient.print(
      String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: api.telegram.org\r\n" +
      "Connection: close\r\n\r\n"
    );
  }

  // ---- Blynk Event ----
  Blynk.logEvent("farm_alert", message);

  lastAlertTime = millis();
}

#endif
