
#include "devicesMints.h"


bool initializeGPGGALRMints()
{
  return true;
}

void readGPGGALRMintsMax()
{
  unsigned long waitTime = 5000;  
  unsigned long start = millis();
  Serial.begin(9600); 
  do 
  {
    while (Serial.available())
      gpggalr.encode(Serial.read());
  } while (millis() - start < waitTime);

SerialUSB.print("Num of Satelites Validity: ");
SerialUSB.println(gpggalr.satellites.isValid());
SerialUSB.print("Num of Satelites: ");
SerialUSB.println(gpggalr.satellites.value());

SerialUSB.print("HDOP Validity: ");
SerialUSB.println(gpggalr.hdop.isValid());
SerialUSB.print("HDOP: ");
SerialUSB.println(gpggalr.hdop.hdop());

SerialUSB.print("Location Validity: ");
SerialUSB.println(gpggalr.location.isValid());
SerialUSB.print("Latitude: ");
SerialUSB.println(gpggalr.location.lat(),10);
SerialUSB.print("Longitude: ");
SerialUSB.println(gpggalr.location.lng(),10);
SerialUSB.print("Location Age: ");
SerialUSB.println(gpggalr.location.age());

SerialUSB.print("Date: ");
SerialUSB.print(gpggalr.date.year());
SerialUSB.print(":");
SerialUSB.print(gpggalr.date.month());
SerialUSB.print(":");
SerialUSB.println(gpggalr.date.day());  

SerialUSB.print("Time: ");
SerialUSB.print(gpggalr.time.hour());
SerialUSB.print(":");
SerialUSB.print(gpggalr.time.minute());
SerialUSB.print(":");
SerialUSB.println(gpggalr.time.second());

SerialUSB.print("Altitude Validity: ");
SerialUSB.println(gpggalr.altitude.isValid());
SerialUSB.print("Altitude: ");
SerialUSB.println(gpggalr.altitude.meters());

SerialUSB.print("Course Validity: ");
SerialUSB.println(gpggalr.course.isValid());
SerialUSB.print("Course: ");
SerialUSB.println(gpggalr.course.deg());

SerialUSB.print("Speed Validity: ");
SerialUSB.println(gpggalr.course.isValid());
SerialUSB.print("Speed: ");
SerialUSB.println(gpggalr.speed.kmph());

SerialUSB.print("Altitude: ");
SerialUSB.println(gpggalr.altitude.meters());

if (gpggalr.location.isValid())
  {
    uint8_t portIn       = 5;
    uint8_t sizeInDouble = 6;
    uint8_t sizeInUint16 = 1;
    uint8_t sizeInUint8  = 5;
    
    String sensorName = "GPGGALR" ;
    double valuesDouble[sizeInDouble]  = {
                   gpggalr.location.lat(),
                   gpggalr.location.lng(),
                   gpggalr.speed.kmph(),
                   gpggalr.altitude.meters(),
                   gpggalr.course.deg(),
                   gpggalr.hdop.hdop()
    };// 42 bytes 

    uint16_t valuesUint16[sizeInUint16]  = {
                 gpggalr.date.year()
    };
    uint8_t valuesUint8[sizeInUint8]  = {
                     gpggalr.date.month(),
                     gpggalr.date.day(),
                     gpggalr.time.hour(),
                     gpggalr.time.minute(),
                     gpggalr.time.second(),
                  };  

  uint8_t sizeInBytesDouble = sizeof(valuesDouble);   
  uint8_t sizeInBytesUint16 = sizeof(valuesUint16);   
  uint8_t sizeInBytesUint8  = sizeof(valuesUint8);
  uint8_t sizeInBytes       =  sizeInBytesDouble + sizeInBytesUint16+ sizeInBytesUint8;

  byte sendOutDouble[sizeInBytesDouble];
  byte sendOutUint16[sizeInBytesUint16];
  byte sendOutUint8[sizeInBytesUint8];
  
  byte sendOut[sizeInBytes];

  memcpy(sendOutDouble,&valuesDouble,sizeof(valuesDouble));
  memcpy(sendOutUint16,&valuesUint16,sizeof(valuesUint16));
  memcpy(sendOutUint8,&valuesUint8,sizeof(valuesUint8));

  memcpy(sendOut, &sendOutDouble, sizeof(valuesDouble));
  memcpy(sendOut + sizeInBytesDouble, &sendOutUint16, sizeof(valuesUint16));
  memcpy(sendOut + sizeInBytesDouble + sizeInBytesUint16, &sendOutUint8 ,sizeof(valuesUint8));

  loRaSendMints(sendOut,sizeInBytes,5,portIn);

 }

}


//  INA 219 
bool initializeINA219DuoMints()
{

  if (ina219Battery.begin()&ina219Solar.begin()) 
  {
        SerialUSB.println("INA219s Initiated");
        delay(1);
        return true;
  }
      else
      {
      SerialUSB.println("INA219s not found");
      delay(1);

      return false;
      }
}



uint8_t getPowerMode(uint8_t powerPin)
{
  float busVoltageBat    = ina219Battery.getBusVoltage_V();
  float busVoltageSol    = ina219Solar.getBusVoltage_V();
  String readings[2] = {  
                          String( busVoltageBat,2), \
                          String( busVoltageSol,2)};
  sensorPrintMints("INA219Duo",readings,2);

  if (abs(busVoltageBat) <= 3.3 ) 
  {
    rebootBoard(powerPin);
    return 0;
  }

  if ((abs(busVoltageBat))> 3.3 && (abs(busVoltageBat)<= 3.5)) {
    return 4;
  }

  if (abs(busVoltageBat) > 4.00)
  {
    if (abs(busVoltageSol) > 5 ){
        return 3;
    }else{
      return 7;
    }
  }


  if (abs(busVoltageBat)> 3.5 && abs(busVoltageBat)<= 4)  {
    if (abs(busVoltageSol)<=5)
    {
      return 5;
    }else
    {
      return 6;
    }
  }
    rebootBoard(powerPin);
    return 0 ;
       
}



uint32_t getPeriod(uint8_t powerMode, String sensorID)
{
  if (sensorID == "IPS7100") {
    if (powerMode == 3){
    return 60000; 
    }else
    {
    return 120000; 
    }
  }
  if (sensorID == "BME280") {
    if (powerMode == 3 ){
    return 60000; 
    }else
    {
    return 120000; 
    }
  }

  if (sensorID == "INA219Duo") {
    if (powerMode == 3 ){
    return 180000; 
    }else
    {
    return 240000; 
    }
  }

  if (sensorID == "GPGGALR") {
    if (powerMode == 3 ){
    return 180000; 
    }else
    {
    return 240000; 
    }
  }
return 3000000;

} // End of Period Seek 
 


void readINA219DuoMints(){

  float shuntVoltageBat  = ina219Battery.getShuntVoltage_mV();
  float busVoltageBat    = ina219Battery.getBusVoltage_V();
  float currentMaBat     = ina219Battery.getCurrent_mA();
  float powerMwBat       = ina219Battery.getPower_mW();
  
  float shuntVoltageSol  = ina219Solar.getShuntVoltage_mV();
  float busVoltageSol    = ina219Solar.getBusVoltage_V();
  float currentMaSol     = ina219Solar.getCurrent_mA();
  float powerMwSol       = ina219Solar.getPower_mW();

  String readings[8] = { String(shuntVoltageBat,2) , String( busVoltageBat,2), String(currentMaBat ,2) , String(powerMwBat,2) ,
  String(shuntVoltageSol,2) , String( busVoltageSol,2), String(currentMaSol ,2) , String(powerMwSol,2)};
  sensorPrintMints("INA219Duo",readings,8);
}

void readINA219DuoMintsMax(){
  uint8_t sizeIn = 8;
  uint8_t portIn = 3;
  String sensorName = "INA219Duo" ;
  float values[sizeIn]  = {
                      ina219Battery.getShuntVoltage_mV(),
                      ina219Battery.getBusVoltage_V(),
                      ina219Battery.getCurrent_mA(),
                      ina219Battery.getPower_mW(),
                      ina219Solar.getShuntVoltage_mV(),
                      ina219Solar.getBusVoltage_V(),
                      ina219Solar.getCurrent_mA(),
                      ina219Solar.getPower_mW()
                      };

  uint8_t sizeInBytes =sizeof(values);                    
  byte sendOut[sizeInBytes];
  memcpy(sendOut,&values,sizeof(values));
  sensorPrintFloats(sensorName,values,sizeIn);
  sensorPrintBytes(sensorName,sendOut,sizeInBytes);
  loRaSendMints(sendOut,sizeInBytes,5,portIn); 
}


//  BME280

bool initializeBME280Mints(){
      if (bme280.init()) {
        SerialUSB.println("BME280 Initiated");
        delay(1);
        return true;
      }
      else
      {
      SerialUSB.println("BME280 not found");
      delay(1);

      return false;
      }

}

void readBME280MintsMax()
{
  uint8_t sizeIn = 3;
  uint8_t portIn = 21;
  String sensorName = "BME280" ;
  float values[sizeIn]  = {
                      bme280.getTemperature(),
                      float(bme280.getPressure())/100,
                      float(bme280.getHumidity())
                      };
  uint8_t sizeInBytes =sizeof(values);                    
  byte sendOut[sizeInBytes];
  memcpy(sendOut,&values,sizeof(values));
  sensorPrintFloats(sensorName,values,sizeIn);
  sensorPrintBytes(sensorName,sendOut,sizeInBytes);
  loRaSendMints(sendOut,sizeInBytes,5,portIn); 
}

// // MGS001  ---------------------------------------

bool initializeMGS001Mints(){

  gas.begin(0x04);//the default I2C address of the slave is 0x04
  gas.powerOn();
  SerialUSB.println("MGS001 Initiated");
  SerialUSB.print("MGS001 Firmware Version = ");
  SerialUSB.println(gas.getVersion());
  delay(1);

return true;
}

void readMGS001MintsMax(){
  uint8_t sizeIn = 8;
  uint8_t portIn = 31;
  String sensorName = "MGS001" ;
  float values[sizeIn]  = {
                        gas.measure_C2H5OH(),
                        gas.measure_C3H8(),
                        gas.measure_C4H10(),
                        gas.measure_CH4(),
                        gas.measure_CO(),
                        gas.measure_H2(),
                        gas.measure_NH3(),
                        gas.measure_NO2()
                        };

  uint8_t sizeInBytes = sizeof(values);                    
  byte sendOut[sizeInBytes];
  memcpy(sendOut,&values,sizeof(values));
  sensorPrintFloats(sensorName,values,sizeIn);
  sensorPrintBytes(sensorName,sendOut,sizeInBytes);
  loRaSendMints(sendOut,sizeInBytes,5,portIn); 
}


// SCD30 ---------------------------------------
bool initializeSCD30Mints(uint16_t scdPeriod ){
  if (scd.begin()) {
    delay(10);
    scd.setMeasurementInterval(scdPeriod);
    delay(1);
    SerialUSB.println("SCD30 Initiated");
    return true;
  }else{
    SerialUSB.println("SCD30 not found");
    delay(1);
    return false;
  }
}

void readSCD30MintsMax(){
  uint8_t sizeIn = 3;
  uint8_t portIn = 33;
  String sensorName = "SCD30" ;
  float values[sizeIn]  = {
                      scd.getCO2(),
                      scd.getTemperature(),
                      scd.getHumidity()
                      };


    uint8_t sizeInBytes = sizeof(values);                    
    byte sendOut[sizeInBytes];
    memcpy(sendOut,&values,sizeof(values));
    sensorPrintFloats(sensorName,values,sizeIn);
    sensorPrintBytes(sensorName,sendOut,sizeInBytes);
    if (values[0] >  0 ){
      loRaSendMints(sendOut,sizeInBytes,5,portIn); 
    }
  }


// // IPS7100 ---------------------------------------
bool initializeIPS7100Mints(){
    // ips_sensor.begin();
    SerialUSB.println("IPS7100 Initiated");
   return true;
}

void readIPS7100MintsMax(){
  ips_sensor.update();
  // ips_sensor.updatePC();
  // ips_sensor.updatePM();
  uint8_t sizeIn = 7;
  uint8_t portIn = 15;
  
  String sensorName = "IPS7100" ;

  unsigned long valuesPC[sizeIn]  = {
                      ips_sensor.getPC01(),
                      ips_sensor.getPC03(),
                      ips_sensor.getPC05(),
                      ips_sensor.getPC10(),
                      ips_sensor.getPC25(),
                      ips_sensor.getPC50(),                      
                      ips_sensor.getPC100()
  };

  float valuesPM[sizeIn]  = {
                      ips_sensor.getPM01(),
                      ips_sensor.getPM03(),
                      ips_sensor.getPM05(), 
                      ips_sensor.getPM10(),
                      ips_sensor.getPM25(),
                      ips_sensor.getPM50(),                      
                      ips_sensor.getPM100()
                      };


  uint8_t sizeInBytesPC = sizeof(valuesPC);   
  uint8_t sizeInBytesPM = sizeof(valuesPM);   
  uint8_t sizeInBytes = sizeof(valuesPC) + sizeof(valuesPM);  

  byte sendOutPC[sizeInBytesPC];
  byte sendOutPM[sizeInBytesPM];
  byte sendOut[sizeInBytes];

  memcpy(sendOutPC,&valuesPC,sizeof(valuesPC));
  memcpy(sendOutPM,&valuesPM,sizeof(valuesPM));



  memcpy(sendOut, &sendOutPC, sizeof(valuesPC));
  memcpy(sendOut + sizeInBytesPC, &sendOutPM, sizeof(valuesPC));

  sensorPrintULongs(sensorName,valuesPC,sizeIn);
        SerialUSB.println(" ");
  sensorPrintFloats(sensorName,valuesPM,sizeIn);
        SerialUSB.println(" ");
  sensorPrintBytes(sensorName,sendOutPC,sizeInBytesPC);
        SerialUSB.println(" ");
  sensorPrintBytes(sensorName,sendOutPM,sizeInBytesPM);
            SerialUSB.println(" ");                 

  sensorPrintBytes(sensorName,sendOut,sizeInBytes);
  
    if (valuesPC[0] >  0 ){
      loRaSendMints(sendOut,sizeInBytes,5,portIn); 
    }

}


void resetIPS7100Mints(uint32_t secondsIn){
  SerialUSB.println("Resetting IPS7100");
  for (uint16_t  cT = 1 ;cT<secondsIn ; cT++){
    ips_sensor.update();
    delay(1010);
  } 
}


