#ifndef DEVICES_MINTS_H
#define DEVICES_MINTS_H

#include "loRaMints.h"
#include "jobsMints.h"


#include "Adafruit_INA219.h"
#include "Seeed_BME280.h"
#include "SparkFun_SCD30_Arduino_Library.h"
#include "MutichannelGasSensor.h"
#include "LoRaWan.h"
#include "TinyGPS++.h"
#include <IpsI2C.h>



extern Adafruit_INA219 ina219Battery;
extern Adafruit_INA219 ina219Solar;
bool initializeINA219DuoMints();
void readINA219DuoMints();
void readINA219DuoMintsMax();

extern TinyGPSPlus gpggalr;
bool initializeGPGGALRMints();
void readGPGGALRMintsMax();


extern BME280 bme280;
bool initializeBME280Mints();
void readBME280Mints();
void readBME280MintsMax();

bool initializeMGS001Mints();
void readMGS001Mints();
void readMGS001MintsMax();

extern SCD30 scd;
bool initializeSCD30Mints(uint16_t scdPeriod );
// void readSCD30Mints();
void readSCD30MintsMax();


extern IpsSensor ips_sensor;
bool initializeIPS7100Mints();
void readIPS7100MintsMax();
void resetIPS7100Mints(uint32_t secondsIn);

uint8_t getPowerMode(uint8_t powerPin);
uint32_t getPeriod(uint8_t getPowerMode, String sensorID);


#endif