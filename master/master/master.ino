#include <Wire.h>
#include <string.h>

#define START 1

unsigned char started = 0;
char buffer[100];
char index = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
    delay(1000);
    Serial.println("Loop");
  if (!started) {
    if (Serial.available() > 0) {
      if (Serial.read() == 1) {
        Serial.println("Lancement du jeu");
         /**
          * TODO : start le jeu
         */
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

        Serial.println("Addr : " + (short int)addr);
        Serial.println("Values : ");
        for(int i = 0; i < index; i++) {
            Serial.println((short int)values[i]);
        }

        /**
         ** Envoyer les valeurs au module addr
        */
        Wire.beginTransmission(addr);
        Wire.write(values, index);
        Wire.endTransmission();
      }
    }
  }
}