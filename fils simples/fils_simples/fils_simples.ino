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

#define OUTPUT_INIT 12

#include <Arduino.h>
#include <Wire.h>



char state = STATE_ATTENTE_CONFIGURATION;

char wireToCut = 0;
char nbWires = 0;

bool flag=false;

byte receivedBytes[BUFFER_SIZE];
bool wiresState[6];

void requestEvent();
void receiveEvent(int);
int addrWire(int);

void setup() {
	Wire.begin(MODULE_ADDR);
	Wire.onRequest(requestEvent);
	Wire.onReceive(receiveEvent);

	Serial.begin(9600);

	for(int i=0; i<6; i++) pinMode(addrWire(i), INPUT_PULLUP);

	pinMode(4, OUTPUT);
	digitalWrite(4, HIGH);

	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);

  pinMode(OUTPUT_INIT, OUTPUT);
  digitalWrite(OUTPUT_INIT, HIGH);
  delay(50);
  digitalWrite(OUTPUT_INIT, LOW);

  state = 3;
}

int addrWire(int wire) {
	switch(wire) {
		case 0: return 5;
		case 1: return 6;
		case 2: return 7;
		case 3: return 8;
		case 4: return 9;
		case 5: return 10;
		default: return -1;
	}
}

void loop() {

  //Serial.print("Module state : ");
  // Serial.println((short int) state);
	/**
	* ! CONFIGURATION DU MODULE
	* Vérifier que les fils soient bien branchés pour que le module soit armé lorsque la solution est reçue
	*/

	if(((state == STATE_ATTENTE_CONFIGURATION) || (state == STATE_PRET)) && (wireToCut > 0 && nbWires >= 3)) {

		flag=true;
		for(int i=0; i<6; i++) {

      /*
			Serial.print("pin : ");
			Serial.print(i+2);
			Serial.print(" ; state : ");
			Serial.println(digitalRead(i+2));
      */

			// if(!digitalRead(i+2) && i >= nbWires) flag=false;
			// else if(digitalRead(i+2) && i < nbWires) flag=false;
			if(!digitalRead(addrWire(i)) && i >= nbWires) {
        flag=false;
      }
			else if(digitalRead(addrWire(i)) && i < nbWires) {
        flag=false;  
      }

		}

		// Si la configuration est conforme, alors on arme le module
		if(flag && state != STATE_RAS && state != STATE_ERREUR) {

      state = STATE_PRET;

			digitalWrite(13, HIGH);
			
			for(int i=0; i<6; i++) {
				// wiresState[i] = digitalRead(i+2);
				wiresState[i] = digitalRead(addrWire(i));
			}

		} 

		// Si la configuration est non conforme, alors on désarme le module
		else if (state == STATE_PRET) {
			state = STATE_ATTENTE_CONFIGURATION;
			digitalWrite(13, LOW);
		}


	}

	/**
	* ! MODULE ARMÉ JEU LANCÉ
	* Vérifier que le bon foit soit débranché 
	*/

  
	else if (state == STATE_RAS) {

		for(int wire=1; wire<=nbWires; wire++){
			// Si le fil est nouvellement débranché (erreur commise)
			/* if(wireToCut != wire && digitalRead(wire+1) && !wiresState[wire-1]) {
				state = STATE_ERREUR;
				return;
			}
			wiresState[wire-1] = digitalRead(wire+1); */

			if(wireToCut != wire && digitalRead(addrWire(wire-1)) && !wiresState[wire-1]) {
        Serial.print("Mise en erreur pour le fil à l'addr");
        Serial.println(addrWire(wire-1));
				state = STATE_ERREUR;
        wiresState[wire-1] = HIGH;
				return;
			}

			// wiresState[wire-1] = digitalRead(addrWire(wire-1));
		}

		// Si le bon fil est débranché
		/* if(digitalRead(wireToCut+1)) {
			state = STATE_DESARME;
			digitalWrite(13, HIGH);
		} */

		if(digitalRead(addrWire(wireToCut-1))) {
			state = STATE_DESARME;
			digitalWrite(13, HIGH);
		}

	}
 


  delay(20);
}

void requestEvent() {
	Wire.write(state);

	if(state == STATE_ERREUR) {
		state = STATE_RAS;
    Serial.println("Remise en state RAS");
	} 
}

void receiveEvent(int size) {
  if(size == 0) return;
  Serial.print("ReceiveEvent, size = ");
  Serial.println(size);
	if(Wire.available() >= size) {
		for(int i=0; i<size; i++) {
			receivedBytes[i] = Wire.read();
      Serial.println(receivedBytes[i]);
		}
    

		/**
		 ** Si lancement du jeu
		 */
		if(size == 1 && receivedBytes[0] == 0 && state == STATE_PRET) {
			state = STATE_RAS;


			digitalWrite(13, LOW);
		}

		/**
		 ** Si configuration du module
		 */
		if(size != 2 || (state != STATE_PRET && state != STATE_ATTENTE_CONFIGURATION)) return;


		for(int i=0; i<size;i++){

			if(i==0){
				wireToCut=receivedBytes[i];
			} else if (i==1) {
				nbWires=receivedBytes[i];
			}

		}

	} else {
		Serial.println("Erreur");
	}
}