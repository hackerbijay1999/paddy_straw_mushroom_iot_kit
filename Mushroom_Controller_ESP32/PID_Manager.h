#ifndef PID_MANAGER_H
#define PID_MANAGER_H

#include <PID_v1.h>
#include "Config.h"

double pidInput, pidOutput;
double pidSetpoint = PID_TARGET_TEMP_C;

/* Tuned for mushroom chamber */
double Kp = 2.0;
double Ki = 0.5;
double Kd = 1.0;

PID tempPID(&pidInput, &pidOutput, &pidSetpoint, Kp, Ki, Kd, DIRECT);

void setupPID() {
  tempPID.SetMode(AUTOMATIC);
  tempPID.SetOutputLimits(0, 100);
}

bool pidHeaterDecision(float temperature) {
  pidInput = temperature;
  tempPID.Compute();
  return pidOutput > 50;   // threshold
}

#endif
