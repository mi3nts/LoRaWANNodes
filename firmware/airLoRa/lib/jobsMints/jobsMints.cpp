#include "jobsMints.h"

void initializeSerialMints(){
    SerialUSB.begin(9600);
    SerialUSB.println("Serial Port Open");
}

void sensorPrintMints(String sensor,String readings[],uint8_t numOfvals){
  SerialUSB.print("#mintsO!");SerialUSB.print(sensor);SerialUSB.print(">");
  for (int i = 0; i < numOfvals; ++i)
      {
        SerialUSB.print(readings[i]);SerialUSB.print(":");
      }
      SerialUSB.print("~");
}








