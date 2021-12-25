#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SimpleModbusSlaveSoftwareSerial.h>
#include <SimpleDHT.h>

int pinDHT11 = 3;
SimpleDHT11 dht11;

#define RX            0
#define TX            1
#define RS485_EN      2
#define BAUD_RATE     19200
#define deviceID      3   

enum  {
  TEMP,
  HUM,
  TOTAL_ERRORS,
  TOTAL_REGS_SIZE 
};

unsigned int holdingRegs[TOTAL_REGS_SIZE];

SoftwareSerial mySerial(RX, TX);

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}