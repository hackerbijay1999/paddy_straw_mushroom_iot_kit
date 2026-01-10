#ifndef PZEM_MANAGER_H
#define PZEM_MANAGER_H

#include <PZEM004Tv30.h>

/* ===== UART CONFIG ===== */
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17

/* ===== PZEM OBJECT ===== */
PZEM004Tv30 pzem(Serial2, PZEM_RX_PIN, PZEM_TX_PIN);

/* ===== POWER DATA ===== */
float voltage = 0.0;
float current = 0.0;
float power   = 0.0;
float energy  = 0.0;

bool pzemHealthy = true;

/* ===== SETUP ===== */
void setupPZEM() {
  Serial2.begin(9600, SERIAL_8N1, PZEM_RX_PIN, PZEM_TX_PIN);
}

/* ===== READ DATA ===== */
void readPZEM() {
  float v = pzem.voltage();
  float c = pzem.current();
  float p = pzem.power();
  float e = pzem.energy();

  if (isnan(v) || isnan(c) || isnan(p)) {
    pzemHealthy = false;
    return;
  }

  voltage = v;
  current = c;
  power   = p;
  energy  = e;
  pzemHealthy = true;
}

#endif
