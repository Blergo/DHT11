#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SimpleModbusSlaveSoftwareSerial.h>
#include <dht.h>
#include <EEPROM.h>

int DHT11_PIN = 2;
dht DHT;
byte temperature = 0;
byte humidity = 0;

#define RX            1
#define TX            0
#define RS485_EN      -1
#define BAUD_RATE     9600
int deviceID = 1;
int NewID;

unsigned long u32wait = millis() + 500;

enum  {
  SET_ID,
  ABILITY,
  ERR_CODE,
  TOTAL_ERRORS,
  TEMPERATURE,
  HUMIDITY,
  TOTAL_REGS_SIZE 
};

unsigned int holdingRegs[TOTAL_REGS_SIZE];

SoftwareSerial mySerial(RX, TX);

void setup() {
  mySerial.begin(BAUD_RATE);
  holdingRegs[1] = 05;
  EEPROM.begin();
  EEPROM.get(0, NewID);
  if (NewID != deviceID){
    deviceID = NewID;
  }
  modbus_configure(&mySerial, BAUD_RATE, deviceID, RS485_EN, TOTAL_REGS_SIZE);
}

void reboot(void) {
  noInterrupts();
  CLKPR = 0b10000000;
  CLKPR = 0;
  void (*ptrToFunction)();
  ptrToFunction = 0x0000;
  (*ptrToFunction)();
}

void loop() {
  if(millis() > u32wait){
    int chk = DHT.read11(DHT11_PIN);
    switch (chk){
      case DHTLIB_OK:  
        holdingRegs[2] = 0;
      break;
      case DHTLIB_ERROR_CHECKSUM: 
        holdingRegs[2] = 1; 
      break;
      case DHTLIB_ERROR_TIMEOUT: 
        holdingRegs[2] = 2; 
      break;
      case DHTLIB_ERROR_CONNECT:
        holdingRegs[2] = 3;
      break;
      case DHTLIB_ERROR_ACK_L:
        holdingRegs[2] = 4;
      break;
      case DHTLIB_ERROR_ACK_H:
        holdingRegs[2] = 5;
      break;
      default: 
        holdingRegs[2] = 6;
      break;
    }
    u32wait = millis() + 500;
  }

  if(holdingRegs[0] != 0){
    EEPROM.put(0, holdingRegs[0]);
    delay(100);
    reboot();
  }
  
  holdingRegs[4] = (int)DHT.temperature;
  holdingRegs[5] = (int)DHT.humidity;
  holdingRegs[3] = modbus_update(holdingRegs);
}