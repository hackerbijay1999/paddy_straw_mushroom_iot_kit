#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include "RTC_Manager.h"
#include "WiFi_Manager.h"

#define BTN_RTC   18
#define BTN_WIFI  19
#define BTN_FREE  33

#define LONG_PRESS_MS   3000
#define BOOT_IGNORE_MS  3000

unsigned long btnRTCPressTime  = 0;
unsigned long btnWiFiPressTime = 0;

bool btnRTCHandled  = false;
bool btnWiFiHandled = false;

unsigned long bootTime = 0;

void setupButtons() {
  pinMode(BTN_RTC, INPUT_PULLUP);
  pinMode(BTN_WIFI, INPUT_PULLUP);
  pinMode(BTN_FREE, INPUT_PULLUP);

  bootTime = millis();
}

/* =========================================================
   BUTTON HANDLER
   ========================================================= */
void handleButtons() {

  /* ===== IGNORE BUTTONS JUST AFTER BOOT ===== */
  if (millis() - bootTime < BOOT_IGNORE_MS) return;

  /* ================= RTC BUTTON ================= */
  if (digitalRead(BTN_RTC) == LOW) {   // PRESSED
    if (btnRTCPressTime == 0) {
      btnRTCPressTime = millis();
    }

    if (!btnRTCHandled && millis() - btnRTCPressTime >= LONG_PRESS_MS) {
      Serial.println("⏰ RTC Sync Triggered");

      syncRTCWithNTP();
      Serial.println(getTimestamp());

      btnRTCHandled = true;
    }
  } else {
    btnRTCPressTime = 0;
    btnRTCHandled = false;
  }

  /* ================= WIFI RESET BUTTON ================= */
  if (digitalRead(BTN_WIFI) == LOW) {
    if (btnWiFiPressTime == 0) {
      btnWiFiPressTime = millis();
    }

    if (!btnWiFiHandled && millis() - btnWiFiPressTime >= LONG_PRESS_MS) {
      Serial.println("📡 WiFi Reset Triggered");
      delay(200);
      resetWiFi();
      btnWiFiHandled = true;
    }
  } else {
    btnWiFiPressTime = 0;
    btnWiFiHandled = false;
  }
}

#endif
