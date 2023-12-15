/**
  * Module Fils Simples

  Documentation I2C :
  Premier octet reçu => solution (numéro du fil entre 1 et 6 à retirer pas l'utilisateur)
  Deuxième octet reçu => nombre de fils à placer par l'utilisateur pour activer le module
  */
#define MODULE_ADDR 1

#define STATE_RAS 0 
#define STATE_DESARME 1 
#define STATE_ERREUR 2
#define STATE_ATTENTE_CONFIGURATION 3
#define STATE_PRET 4

#define BUFFER_SIZE 4
#include <Wire.h>

char state = 3;

char wireToCut = 0;
char nbWires = 0;

int wireRemoved1 = 0;
int wireRemoved2 = 0;

bool flag=false;

byte receivedBytes[BUFFER_SIZE];

void requestEvent();
void receiveEvent(int);

void setup() {
  // put your setup code here, to run once:
  Wire.begin(MODULE_ADDR);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  Serial.begin(9600);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {
  /**
    * ! CONFIGURATION DU MODULE
    */
  if(state == STATE_ATTENTE_CONFIGURATION && wireToCut > 0 && nbWires >= 3) {
    flag=true;
    for(int pin=2; pin<=6; pin++) {
      Serial.print("pin : ");
      Serial.print(pin);
      Serial.print(" ; state : ");
      Serial.println(digitalRead(pin));
      if(!digitalRead(pin) && pin-1 > nbWires) {
        flag=false;
      }
      else if(digitalRead(pin) && pin-1 <= nbWires) flag=false;
    }

    if(flag) {
      flag=false;
      digitalWrite(13, HIGH);
      Serial.println("Led allumée");
      state=STATE_PRET;
    }
  }

  /**
    * ! MODULE ARME
    */
  else if (state == STATE_RAS) {

    // Si le bon fil est débranché
    if(digitalRead(wireToCut)) {
      state = STATE_DESARME;
    }

    for(int wire=1; wire<=nbWires; wire++){
      // Si le fil est nouvellement débranché (erreur commise)
      if(digitalRead(wire+1) && wire != wireToCut) {
        
      }
    }

  }


  delay(500);
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
    for(int i=0; i<size;i++){
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