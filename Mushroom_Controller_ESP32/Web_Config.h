#ifndef WEB_CONFIG_H
#define WEB_CONFIG_H

#include <WebServer.h>
#include "Globals.h"
#include "Storage_Manager.h"
#include "Relay_Manager.h"

WebServer server(80);

/* ================= BASIC UI ================= */
String htmlHeader =
"<html><head><style>"
"body{font-family:Arial;background:#f4f6f8;padding:20px;}"
".card{background:white;padding:15px;border-radius:8px;max-width:420px;margin:auto;}"
"h2{text-align:center;}"
"input,button{width:100%;padding:10px;margin:6px 0;font-size:15px;}"
".ok{background:#4CAF50;color:white;}"
".warn{background:#f44336;color:white;}"
".info{background:#2196F3;color:white;}"
"</style></head><body>";

/* ================= LOGIN PAGE ================= */
String loginPage() {
  return htmlHeader +
    "<div class='card'><h2>Login</h2>"
    "<form method='POST' action='/login'>"
    "<input name='user' placeholder='Username'>"
    "<input type='password' name='pass' placeholder='Password'>"
    "<button class='ok'>Login</button>"
    "</form></div></body></html>";
}

/* ================= SESSION CHECK ================= */
bool ensureSession() {
  if (!sessionActive || currentUser == ROLE_NONE) {
    return false;
  }
  lastActivityTime = millis();   // 🔥 refresh session on EVERY request
  return true;
}

/* ================= LOGIN ================= */
void handleLogin() {
  String u = server.arg("user");
  String p = server.arg("pass");

  if (u == WEB_ADMIN_USER && p == WEB_ADMIN_PASS) {
    currentUser = ROLE_ADMIN;
  } 
  else if (u == WEB_OPERATOR_USER && p == WEB_OPERATOR_PASS) {
    currentUser = ROLE_OPERATOR;
  } 
  else {
    server.send(401, "text/plain", "Invalid credentials");
    return;
  }

  sessionActive = true;
  lastActivityTime = millis();

  server.sendHeader("Location", "/");
  server.send(302);
}

/* ================= LOGOUT ================= */
void handleLogout() {
  currentUser = ROLE_NONE;
  sessionActive = false;
  lastActivityTime = 0;
  server.sendHeader("Location", "/");
  server.send(302);
}

/* ================= ROOT ================= */
void handleRoot() {

  if (!ensureSession()) {
    server.send(200, "text/html", loginPage());
    return;
  }

  String page = htmlHeader +
    "<div class='card'><h2>Mushroom Controller</h2>"
    "<p><b>Mode:</b> " +
    String(emergencyStop ? "EMERGENCY" :
           (currentMode == AUTO_MODE ? "AUTO" : "MANUAL")) +
    "</p>"
    "<p><b>Target Temp:</b> " + String(targetTempC) + " °C</p>";

  if (currentUser == ROLE_ADMIN) {
    page +=
      "<form action='/set'>"
      "<input name='temp' placeholder='Target Temp'>"
      "<button class='ok'>Update Temp</button></form>"

      "<a href='/mode?set=auto'><button class='info'>AUTO</button></a>"
      "<a href='/mode?set=manual'><button class='info'>MANUAL</button></a>"

      "<a href='/mqtt'><button class='info'>MQTT Settings</button></a>";
  }

  page +=
    "<a href='/relay?ch=0&state=on'><button class='warn'>Heater ON</button></a>"
    "<a href='/relay?ch=0&state=off'><button class='warn'>Heater OFF</button></a>"
    "<a href='/logout'><button>Logout</button></a>"
    "</div></body></html>";

  server.send(200, "text/html", page);
}

/* ================= MQTT CONFIG PAGE ================= */
void handleMQTTPage() {

  if (!ensureSession() || currentUser != ROLE_ADMIN) {
    server.send(403, "text/plain", "Forbidden");
    return;
  }

  String page = htmlHeader +
    "<div class='card'><h2>MQTT Settings</h2>"
    "<form method='POST' action='/mqtt/save'>"
    "<input name='broker' value='" + String(mqttBroker) + "' placeholder='Broker'>"
    "<input name='port' value='" + String(mqttPort) + "' placeholder='Port'>"
    "<input name='topic' value='" + String(mqttBaseTopic) + "' placeholder='Base Topic'>"
    "<button class='ok'>Save & Reboot</button>"
    "</form>"
    "<a href='/'><button>Back</button></a>"
    "</div></body></html>";

  server.send(200, "text/html", page);
}

/* ================= SAVE MQTT ================= */
void handleMQTTSave() {

  if (!ensureSession() || currentUser != ROLE_ADMIN) return;

  strlcpy(mqttBroker, server.arg("broker").c_str(), sizeof(mqttBroker));
  mqttPort = server.arg("port").toInt();
  strlcpy(mqttBaseTopic, server.arg("topic").c_str(), sizeof(mqttBaseTopic));

  saveConfig();

  server.send(200, "text/html",
    htmlHeader +
    "<div class='card'><h2>Saved</h2>"
    "<p>Rebooting device...</p>"
    "</div></body></html>");

  delay(1000);
  ESP.restart();
}

/* ================= SET TEMP ================= */
void handleSetTemp() {
  if (!ensureSession() || currentUser != ROLE_ADMIN) return;
  targetTempC = server.arg("temp").toFloat();
  saveConfig();
  server.sendHeader("Location", "/");
  server.send(302);
}

/* ================= MODE ================= */
void handleMode() {
  if (!ensureSession() || currentUser != ROLE_ADMIN) return;
  currentMode = (server.arg("set") == "auto") ? AUTO_MODE : MANUAL_MODE;
  saveConfig();
  server.sendHeader("Location", "/");
  server.send(302);
}

/* ================= RELAY ================= */
void handleRelay() {
  if (!ensureSession()) return;

  int ch = server.arg("ch").toInt();
  bool state = server.arg("state") == "on";
  setRelay(ch, state);
  saveConfig();

  server.sendHeader("Location", "/");
  server.send(302);
}

/* ================= SERVER ================= */
void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/logout", handleLogout);
  server.on("/set", handleSetTemp);
  server.on("/mode", handleMode);
  server.on("/relay", handleRelay);
  server.on("/mqtt", handleMQTTPage);
  server.on("/mqtt/save", HTTP_POST, handleMQTTSave);
  server.begin();
}

void handleWebServer() {
  server.handleClient();
}

#endif
