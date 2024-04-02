#include <Wire.h> 
#include <Arduino.h>
#include <TM1637Display.h>
#include "afficheur.h"

#define CLK 6
#define DIO 5

#define START 1

#define ID_MAX_MODULE 5 // ID du dernier module existant (commence à 1)

bool started = false;
unsigned char buffer[100];
unsigned char index = 0;
unsigned char modulesConnected[ID_MAX_MODULE+1];


void scanModules(unsigned char * modules);
bool isModuleConnected(unsigned char);
unsigned char getModuleState(unsigned char module_addr);

void setup() {

  for(unsigned char i=1; i<ID_MAX_MODULE; i++) modulesConnected[i] = 0;

  Wire.begin();

	Serial.begin(9600);
	// Serial.println("KTANE");

  TM1637Display display(CLK, DIO);

  display.setBrightness(0x0f);
  display.setSegments(defu);
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
				for(int i = 1; i <= ID_MAX_MODULE; i++) {
					if(isModuleConnected(i) && getModuleState(i) == 4) {
						started = true;
					} else {
						started = false;
						break;
					}
				}

				if(started) {
					Serial.println("Jeu lancé");
					for(int i = 1; i <= ID_MAX_MODULE; i++) {
						if(isModuleConnected(i) && getModuleState(i) == 4) {
							
						}
					}
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

		scanModules(modulesConnected);

    delay(1000);
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
			modules[addr] = 0;
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