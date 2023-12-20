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

bool flag=false;

byte receivedBytes[BUFFER_SIZE];
bool wiresState[6];

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
    for(int i=0; i<6; i++) {
      Serial.print("pin : ");
      Serial.print(i+2);
      Serial.print(" ; state : ");
      Serial.println(digitalRead(i+2));
      if(!digitalRead(i+2) && i >= nbWires) {
        flag=false;
      }
      else if(digitalRead(i+2) && i < nbWires) flag=false;
    }

    // Si la configuration est bonne
    if(flag) {
      flag=false;
      digitalWrite(13, HIGH);
      Serial.println("Led allumée");
      state=STATE_PRET;

      for(int i=0; i<6; i++) {
        wiresState[i] = digitalRead(i+2);
      }
    }
  }

  /**
    * ! MODULE ARME
    */
  else if (state == STATE_RAS) {

    // Si le bon fil est débranché
    if(digitalRead(wireToCut)) {
      state = STATE_DESARME;
      digitalWrite(13, HIGH);
    }

    for(int wire=1; wire<=nbWires; wire++){
      // Si le fil est nouvellement débranché (erreur commise)
      if(wireToCut != wire && digitalRead(wire+1) && !wiresState[wire]) {
        state = STATE_ERREUR;
      }
      wiresState[wire] = digitalRead(wire+1);
    }

  }


  delay(100);
}

void requestEvent() {
  Wire.write(state);

  if(state == STATE_ERREUR) {
    state = STATE_RAS;
  } 
}

void receiveEvent(int size) {
  if(Wire.available() >= size) {
    for(int i=0; i<size; i++) {
      receivedBytes[i] = Wire.read();
    }
    

    /**
      * Détection du lancement
      */
    if(size > 0 && receivedBytes[0] == 0 && state == STATE_PRET) {
      state = STATE_RAS;
      digitalWrite(13, LOW);
      return;
    }

    /**
      * Si configuration du module
      */
    if(size != 2) return;
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