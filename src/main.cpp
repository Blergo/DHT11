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

  DHT.read11(DHT11_PIN);

  holdingRegs[0] = (int)DHT.temperature;
  holdingRegs[1] = (int)DHT.humidity;
  holdingRegs[TOTAL_ERRORS] = modbus_update(holdingRegs);
  delay(2000);
}