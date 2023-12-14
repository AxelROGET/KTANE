/**
  * Module Fils Simples

  Documentation I2C :
  Premier octet reçu => solution (numéro du fil entre 1 et 6 à retirer pas l'utilisateur)
  Deuxième octet reçu => nombre de fils à placer par l'utilisateur pour activer le module
  */
#define MODULE_ADDR 1

#define BUFFER_SIZE 4
#include <Wire.h>

char state = 3;

char wireToCut = 0;
char nbWires = 0;

byte receivedBytes[BUFFER_SIZE];

void requestEvent();
void receiveEvent(int);

void setup() {
  // put your setup code here, to run once:
  Wire.begin(MODULE_ADDR);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(20);
}

void requestEvent() {
  Wire.write(state);
}

void receiveEvent(int size) {
  if(size < 2) return;
  if(Wire.available() >= size) {
    for(int i=0; i<size; i++) {
      receivedBytes[i] = Wire.read();
    }
    //Serial.print("Octets reçus : ");
    for(int i=0; i<size;i++){
      /* Serial.print(i);
      Serial.print(" : ");
      Serial.print(receivedBytes[i], HEX);
      Serial.print("; "); */
      if(i==0){
        wireToCut=receivedBytes[i];
      } else if (i==1) {
        nbWires=receivedBytes[i];
      }

    }
    // Serial.println();
  } else {
    Serial.println("Erreur : pas assez d'octets pour remplir le tableau.");
  }
}