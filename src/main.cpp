#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SimpleModbusSlaveSoftwareSerial.h>
#include <dht.h>

int DHT11_PIN = 2;
dht DHT;
byte temperature = 0;
byte humidity = 0;

#define RX            1
#define TX            0
#define RS485_EN      -1
#define BAUD_RATE     9600
#define deviceID      1

enum  {
  TEMP,
  HUM,
  ERR,
  ID,
  TOTAL_ERRORS,
  TOTAL_REGS_SIZE 
};

unsigned int holdingRegs[TOTAL_REGS_SIZE];

SoftwareSerial mySerial(RX, TX);

void setup() {
  mySerial.begin(19200);
  modbus_configure(&mySerial, BAUD_RATE, deviceID, RS485_EN, TOTAL_REGS_SIZE);
}

void loop() {

    int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
    holdingRegs[3] = 0;
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    holdingRegs[3] = 1; 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    holdingRegs[3] = 2; 
    break;
    case DHTLIB_ERROR_CONNECT:
    holdingRegs[3] = 3;
    break;
    case DHTLIB_ERROR_ACK_L:
    holdingRegs[3] = 4;
    break;
    case DHTLIB_ERROR_ACK_H:
    holdingRegs[3] = 5;
    break;
    default: 
    holdingRegs[3] = 6;
    break;
  }

  holdingRegs[0] = (int)DHT.temperature;
  holdingRegs[1] = (int)DHT.humidity;
  holdingRegs[TOTAL_ERRORS] = modbus_update(holdingRegs);
  delay(1500);
}