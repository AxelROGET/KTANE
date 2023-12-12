/**
  * Module Fils Simples
  */
#define MODULE_ADDR 1

#include <Wire.h>

char state = 3;

void requestEvent();
void receiveEvent(int);

void setup() {
  // put your setup code here, to run once:
  Wire.begin(MODULE_ADDR);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(20);
}

void requestEvent() {
  Wire.write(state);
}

void receiveEvent(int size) {
  
}