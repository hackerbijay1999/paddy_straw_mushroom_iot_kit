#ifndef RTC_MANAGER_H
#define RTC_MANAGER_H

#include <Wire.h>
#include <RTClib.h>
#include <WiFi.h>
#include <time.h>

RTC_DS3231 rtc;

/* ===== FORWARD DECLARATION ===== */
String getTimestamp();

/* ===== SETUP RTC ===== */
void setupRTC() {
  Wire.begin(21, 22);
  if (!rtc.begin()) {
    Serial.println("❌ RTC not found");
  } else {
    Serial.println("✅ RTC detected");
  }
}

/* ===== SYNC RTC WITH NTP ===== */
bool syncRTCWithNTP() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ NTP sync failed: WiFi not connected");
    return false;
  }

  configTime(19800, 0, "pool.ntp.org", "time.nist.gov"); // IST = +5:30
  struct tm timeinfo;

  Serial.println("⏳ Waiting for NTP time...");

  if (!getLocalTime(&timeinfo, 10000)) {
    Serial.println("❌ NTP sync failed");
    return false;
  }

  rtc.adjust(DateTime(
    timeinfo.tm_year + 1900,
    timeinfo.tm_mon + 1,
    timeinfo.tm_mday,
    timeinfo.tm_hour,
    timeinfo.tm_min,
    timeinfo.tm_sec
  ));

  Serial.println("✅ RTC synced with NTP");
  Serial.println(getTimestamp());

  return true;
}

/* ===== GET TIMESTAMP STRING ===== */
String getTimestamp() {
  DateTime now = rtc.now();
  char buf[25];
  sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
          now.year(), now.month(), now.day(),
          now.hour(), now.minute(), now.second());
  return String(buf);
}

#endif
