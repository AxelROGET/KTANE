#include <Wire.h> 
#include <Arduino.h>
#include <TM1637Display.h>
#include "afficheur.h"

// A5 -> SCL
// A4 -> SDA

#define CLK 6
#define DIO 5
#define BUZZER 7

#define LED_ERREUR_1 3
#define LED_ERREUR_2 4


#define STATE_RAS 0 
#define STATE_DESARME 1 
#define STATE_ERREUR 2
#define STATE_ATTENTE_CONFIGURATION 3
#define STATE_PRET 4
#define STATE_DECONNECTE 255

#define START 1

#define ID_MAX_MODULE 5 // ID du dernier module existant (commence à 1)

bool started = false;
unsigned char buffer[100];
unsigned char index = 0;
unsigned char modulesConnected[ID_MAX_MODULE+1];
// Variables relatives au jeu
unsigned char nbErreurs = 0;
unsigned char nbMaxErreurs = 2;
int nbSecondes = 180;
unsigned int dureeSeconde = 1000;
unsigned int erreurs = 0; // Explosion si le nombre d'erreurs dépasse 2

// Variables de temps 
unsigned long lastScanModules = 0;
unsigned long lastRefreshAfficheur = 0;




void scanModules(unsigned char * modules);
bool isModuleConnected(unsigned char);
unsigned char getModuleState(unsigned char module_addr);
void updateModulesState(unsigned char * modules);
void gameOver();


TM1637Display display(CLK, DIO);

void setup() {

  for(unsigned char i=1; i<ID_MAX_MODULE; i++) modulesConnected[i] = STATE_DECONNECTE;

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_ERREUR_1, OUTPUT);
  pinMode(LED_ERREUR_2, OUTPUT);

  Wire.begin();

	Serial.begin(9600);
	// Serial.println("KTANE");

  display.setBrightness(0x0f);
  display.setSegments(defu);

  Wire.setWireTimeout(500, true);
}

void loop() {
	if (!started) {
		if (Serial.available() > 0) {
			if (Serial.read() == 1) {
				Serial.println("Lancement du jeu");
				/**
				* TODO : start le jeu
				* Scan des modules connectés
				* Lancer si tous les modules sont prêts (état 4)
				*/
        		started = true;
				for(int i = 1; i <= ID_MAX_MODULE; i++) {
					if(isModuleConnected(i) && getModuleState(i) != STATE_PRET) {
						started = false;
						Serial.print("Lancement impossible. Cause : module ");
						Serial.println(i);
					}
				}

				// * Lancer le jeu
				if(started) {
					Serial.println("Jeu lancé");
					for(int i = 1; i <= ID_MAX_MODULE; i++) {
						if(isModuleConnected(i) && getModuleState(i) == 4) {
							Wire.beginTransmission(i);
							Wire.write(0);
							Wire.endTransmission();
						}
					}
					lastRefreshAfficheur = millis();
					lastScanModules = millis();
				} else {
					Serial.println("Jeu non lancé");
				}

			} else {
				/**
				 * ! Mode configuration
				*/
				Serial.println("Mode configuration");
				unsigned char addr = Serial.read();
				unsigned char values[10];
				unsigned char index = 0;
				while(Serial.available() > 0 && index < 10) {
						values[index] = Serial.read();
						index++;
				}

				if(addr == 0) {
					// ! Configuration du module Maître
					// * SendSerial(0, 0, <mode>, <value>)
					// mode 0 : nombre de secondes/30
					// mode 1 : nombre max d'erreurs (0-2)

					if(values[0] == 0) {
						nbSecondes = (int) values[1] * 30;
						Serial.print("Nombre de secondes : ");
						Serial.println(nbSecondes);

						// * Actualiser l'afficheur
						int min = nbSecondes / 60;
						int sec = nbSecondes % 60;
						display.showNumberDecEx(min*100 + sec, 0b11100000, true);
					} else if(values[0] == 1) {
						nbMaxErreurs = values[1];
						Serial.print("Nombre max d'erreurs : ");
						Serial.println((short int)nbMaxErreurs);

						// * Actualiser les LEDs
						digitalWrite(LED_ERREUR_1, LOW);
						digitalWrite(LED_ERREUR_2, LOW);
						if(nbMaxErreurs <= 1) digitalWrite(LED_ERREUR_1, HIGH);
						if(nbMaxErreurs == 0) digitalWrite(LED_ERREUR_2, HIGH);

					}
				} else {
					Serial.print("Addr : ");
					Serial.println((short int)addr);
					Serial.println("Values : ");
					for(int i = 1; i < index; i++) {
							Serial.println((short int)values[i]);
					}

					/**
					 ** Envoyer les valeurs au module addr
					* TODO : vérifier que le module est bien connecté en I2C 
					*/
					Wire.beginTransmission(addr);
					Wire.write(values, index);
					Wire.endTransmission();
				}
			}
		}

		scanModules(modulesConnected);

    delay(1000);
	} else {

		// * Scan de tous les modules connectés & configurés (max 10 fois / seconde)
		// Si tous les modules sont à l'état 1 : fin du jeu + afficher "Defu" sur l'afficheur 
		// Si un des modules est à l'état Erreur : incrémenter le nb d'erreurs 

		if(millis() - lastScanModules > 100) {
			updateModulesState(modulesConnected);
			lastScanModules = millis();

			bool defused = true;

			for(unsigned char i=1; i<=ID_MAX_MODULE; i++) {
				if(modulesConnected[i] == STATE_ERREUR) {
					nbErreurs++;

					/* if(nbErreurs == 1) {
						digitalWrite(LED_ERREUR_1, HIGH);
						dureeSeconde = 850;
					} else if(nbErreurs == 2) {
						dureeSeconde = 700;
						digitalWrite(LED_ERREUR_2, HIGH);
					} else if(nbErreurs > 2) {
						gameOver();
						return;
					} */

					char errRestantes = nbMaxErreurs - nbErreurs;

					if(errRestantes < 0) {
						gameOver();
						return;
					}

					if(errRestantes == 0) {
						digitalWrite(LED_ERREUR_1, HIGH);
						digitalWrite(LED_ERREUR_2, HIGH);
					} else if(errRestantes == 1) {
						digitalWrite(LED_ERREUR_1, HIGH);
						digitalWrite(LED_ERREUR_2, LOW);
					} else if(errRestantes == 2) {
						digitalWrite(LED_ERREUR_1, LOW);
						digitalWrite(LED_ERREUR_2, LOW);
					}

					dureeSeconde = 1000 - nbErreurs * (450 - 150 * nbMaxErreurs);



				} 
				
				if(modulesConnected[i] != STATE_DESARME && modulesConnected[i] != STATE_DECONNECTE) {
					defused = false;
					/* Serial.print("Module non désarmé : ");
					Serial.print(i);
					Serial.print(" - ");
					Serial.println(modulesConnected[i]); */
				}
			}

			if(defused) {
				display.setSegments(defu);
				Serial.println("Defused");
				started = false;
				return;
			}
		}

		// * décrémenter le nombre de secondes et actualiser l'affichage toutes les dureeSeconde
		if(millis() - lastRefreshAfficheur > dureeSeconde) {

			if(nbSecondes == 0) {
				gameOver();
				return;
			}

			nbSecondes--;
			lastRefreshAfficheur += dureeSeconde;
			// Passer les secondes en min:sec
			int min = nbSecondes / 60;
			int sec = nbSecondes % 60;
			display.showNumberDecEx(min*100 + sec, 0b11100000, true);
			// Activer le buzzer un court instant
			digitalWrite(BUZZER, HIGH);
			delay(20);
			digitalWrite(BUZZER, LOW);
		}

  }
}

void updateModulesState(unsigned char * modules) {
	for(unsigned char addr=1; addr <= ID_MAX_MODULE; addr++) {
		// Si le module était connecté et l'est toujours, mettre à jour son état
		// Si le module n'était pas connecté, le laisser en état STATE_DECONNECTE
		if(modules[addr] != STATE_DECONNECTE && isModuleConnected(addr)) {
			modules[addr] = getModuleState(addr);
		}
		
		/* else if(modules[addr] != STATE_DECONNECTE && !isModuleConnected(addr)) {
			modules[addr] = STATE_ERREUR;
		}  */
	}
}

void gameOver() {
	display.setSegments(lost);
	Serial.println("Explosion");
	started = false;
	for(int i=0; i<5; i++) {
		digitalWrite(BUZZER, HIGH);
		delay(50);
		digitalWrite(BUZZER, LOW);
		delay(100);
	}
}

void scanModules(unsigned char * modules) {

	//Serial.println("Lancement du scan des modules");

	Serial.print("{ \"modules\": {");

	for(unsigned char addr=1; addr <= ID_MAX_MODULE; addr++) {
    Serial.print("\"");
    Serial.print((short int)addr);
    Serial.print("\":");
		if(isModuleConnected(addr)) {
			modules[addr] = 1;
      Serial.print("\"");
      Serial.print(getModuleState(addr));
      Serial.print("\"");
		} else {
			modules[addr] = STATE_DECONNECTE;
      Serial.print("\"-1\"");
		}

    if(addr < ID_MAX_MODULE) Serial.print(",");
	}

	Serial.println("}}");
	
}

bool isModuleConnected(unsigned char module_addr){
  Wire.beginTransmission(module_addr); 
  if(Wire.endTransmission() == 0) {
    return true;
  } 
  return false;
}

unsigned char getModuleState(unsigned char module_addr) {
	Wire.requestFrom((int)module_addr, 1);
	return Wire.read();
}