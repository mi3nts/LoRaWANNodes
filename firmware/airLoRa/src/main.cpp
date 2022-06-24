
#include "jobsMints.h"
#include "devicesMints.h"
#include "loRaMints.h"
#include "LoRaWan.h"

// ADD LORA APP KEY HERE 
char* keyIn = "0123456789ABCDEF0123456789ABCDEF";

uint8_t numOfTries = 20; 
uint8_t numOfCycles = 0; 

bool GPGGALROnline;
TinyGPSPlus gpggalr;


bool BME280Online;
BME280 bme280; // I2C


bool IPS7100Online;
IpsSensor ips_sensor;
uint32_t IPS7100ResetTime = 30;

bool INA219DuoOnline;

Adafruit_INA219 ina219Battery(0x40);
Adafruit_INA219 ina219Solar(0x41);

uint8_t rebootPin = 9; 
uint8_t powerMode ;

uint32_t BME280Time    = 0;
uint32_t SCD30Time     = 0;
uint32_t MGS001Time    = 0;
uint32_t IPS7100Time   = 0;
uint32_t GPGGALRTime   = 0;
uint32_t INA219DuoTime = 0;

uint32_t BME280Period    ;
uint32_t SCD30Period     ;
uint32_t MGS001Period    ;
uint32_t IPS7100Period   ;
uint32_t GPGGALRPeriod   ;
uint32_t INA219DuoPeriod ;

bool initial = true;

void setup()
{ 
  initializeSerialMints();
  initializeReboot(rebootPin);
  delay(10000);
  INA219DuoOnline =  initializeINA219DuoMints();
  powerMode       =  getPowerMode(rebootPin);
  SerialUSB.print("");
  SerialUSB.print("Power Mode: ");
  SerialUSB.println(powerMode);
  
  INA219DuoPeriod  = getPeriod(powerMode, "INA219Duo");
  SerialUSB.print("INA219Duo Period: ");
  SerialUSB.println(INA219DuoPeriod);

  loraInitMints(keyIn,powerMode,rebootPin);

  GPGGALROnline  = initializeGPGGALRMints();
  GPGGALRPeriod  = getPeriod(powerMode, "GPGGALR");
  SerialUSB.print("GPGGALR Period: ");
  SerialUSB.println(GPGGALRPeriod);

  BME280Online  = initializeBME280Mints();
  BME280Period  = getPeriod(powerMode, "BME280");
  SerialUSB.print("BME280 Period: ");
  SerialUSB.println(BME280Period);

  IPS7100Online =  initializeIPS7100Mints();
  IPS7100Period  = getPeriod(powerMode, "IPS7100");
  SerialUSB.print("IPS7100 Period: ");
  SerialUSB.println(IPS7100Period);

  SerialUSB.print("Power Mode: ");
  SerialUSB.println(powerMode);

  resetIPS7100Mints(IPS7100ResetTime);
  resetLoRaMints(numOfTries,powerMode);
  delay(5000);
}



void loop()
{
      if(readNow(IPS7100Online,IPS7100Time,IPS7100Period,initial))
      {
        readIPS7100MintsMax();
        IPS7100Time  = millis();
        numOfCycles++; 
        delay(5000);
      }
      
      if(readNow(BME280Online,BME280Time,BME280Period,initial))
      { 
        readBME280MintsMax();
        BME280Time  = millis();
        delay(5000);
      }
      
      if(readNow(INA219DuoOnline,INA219DuoTime,INA219DuoPeriod,initial))
      { 
        readINA219DuoMintsMax();
        INA219DuoTime  = millis();
         delay(5000); 
      }

      if(readNow(GPGGALROnline,GPGGALRTime,GPGGALRPeriod,initial))
      { 
        readGPGGALRMintsMax();
        GPGGALRTime  = millis();
        delay(5000); 
      }
     

    initial = false; 

    checkRebootCycle(powerMode,rebootPin,numOfCycles);
}


