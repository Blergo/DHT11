#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SimpleModbusSlaveSoftwareSerial.h>
#include <SimpleDHT.h>

int pinDHT11 = 3;
SimpleDHT11 dht11;

#define RX            0
#define TX            1
#define RS485_EN      -1
#define BAUD_RATE     9600
#define deviceID      247   

enum  {
  TEMP,
  HUM,
  TOTAL_ERRORS,
  TOTAL_REGS_SIZE 
};

unsigned int holdingRegs[TOTAL_REGS_SIZE];

SoftwareSerial mySerial(RX, TX);

void setup() {
  modbus_configure(&mySerial, BAUD_RATE, deviceID, RS485_EN, TOTAL_REGS_SIZE);
}

void loop() {
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    holdingRegs[0] = 0;
    holdingRegs[1] = 0;
    return;
  }

  holdingRegs[0] = (int)temperature;
  holdingRegs[1] = (int)humidity;
  holdingRegs[TOTAL_ERRORS] = modbus_update(holdingRegs);
}