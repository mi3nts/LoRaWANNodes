#ifndef JOBS_MINTS_H
#define JOBS_MINTS_H

#include <Arduino.h>

void initializeSerialMints();
void sensorPrintMints(String sensor,String readings[],uint8_t numOfvals);

#endif