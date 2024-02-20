/**
* Module Clavier
  Données reçues : 4 octets, correspondant à l'ordre dans lequel il faut appuyer sur les touches
  Exemple : 4,2,3,1 => on doit appuyer sur le bouton 4, puis 2, puis 3 et enfin 1
*/
#include <Arduino.h>
#include <Wire.h>

#define BOUTON1 9
#define BOUTON2 10
#define BOUTON3 11
#define BOUTON4 12
#define LED 8

#define MODULE_ADDR 2
#define BUFFER_SIZE 4

#define STATE_RAS 0
#define STATE_DESARME 1 
#define STATE_ERREUR 2
#define STATE_ATTENTE_CONFIGURATION 3
#define STATE_PRET 4



void requestEvent();
void receiveEvent(int);
int boutonAppuye();

char state;
byte receivedBytes[BUFFER_SIZE];
int touchesAppuyees[] = {-1,-1,-1,-1};
int ordreTouches[] = {-1,-1,-1,-1};
int nbTouchesAppuyees = 0;
int appui; //touche sur laquelle on a appuyé
int flag=1; //flag pour savoir si erreur ou non
//int flag2=1;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(MODULE_ADDR);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  Serial.begin(9600);

  //les 4 touches
  pinMode(BOUTON1,INPUT_PULLUP);
  pinMode(BOUTON2,INPUT_PULLUP);
  pinMode(BOUTON3,INPUT_PULLUP);
  pinMode(BOUTON4,INPUT_PULLUP);

  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);

  state = STATE_ATTENTE_CONFIGURATION;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(state == STATE_RAS){
    flag = 1;
    if((appui = boutonAppuye()) != 0){ //si appui sur un bouton
      //flag2 = 0;
      Serial.print("Appui touche : ");
      Serial.println(appui);
      touchesAppuyees[nbTouchesAppuyees] = appui; //on enregistre l'appui


      //debug
      Serial.println("touchesAppuyees : ");
      for(int i = 0; i < 4; i++){
        Serial.print(touchesAppuyees[i]);
      }

      Serial.println("");
      Serial.println("ordreTouches : ");
      for(int i = 0; i < 4; i++){
        Serial.print(ordreTouches[i]);
      }

      Serial.println("");
      Serial.print("nb touches : ");
      Serial.println(nbTouchesAppuyees);
      // fin debug


      nbTouchesAppuyees++;
      for(int j = 0; j < nbTouchesAppuyees; j++){
        Serial.print("j : ");
        Serial.println(j);
        if(touchesAppuyees[j] != ordreTouches[j]){
          Serial.print("Erreur appui touche à ");
          Serial.println(j);
          Serial.print("touche touchesAppuyees : ");
          Serial.println(touchesAppuyees[j]);
          Serial.print("touche ordreTouches : ");
          Serial.println(ordreTouches[j]);
          state = STATE_ERREUR;
          flag = 0; //flag erreur 
          break;
        }
      }
      Serial.println("sortie");
      if(touchesAppuyees[3] != -1 && flag){ //premier cas : module désarmé
        Serial.println("Module desarmé");
        state = STATE_DESARME;
        digitalWrite(LED,HIGH);
      } else if(flag){ //deuxième cas : touche ok
        Serial.println("Touche ok");
      } else { //troisième cas : erreur (réinitialiser le tableau touchesAppuyees pour recommencer)
        for(int i=0; i<4; i++){
          touchesAppuyees[i] = -1;
        }
        nbTouchesAppuyees = 0;
      }
      while(boutonAppuye() != 0); //on attend que l'utilisateur relâche le bouton
      delay(100);
      //flag2 = 1;
    }
  }
}

void requestEvent() {
	Wire.write(state);

  if(state == STATE_ERREUR) {
		state = STATE_RAS;
	} 
}

void receiveEvent(int size) {
  /* Serial.print("ReceiveEvent, size = ");
  Serial.println(size); */
	if(Wire.available() >= size) {
		for(int i=0; i<size; i++) {
			receivedBytes[i] = Wire.read();
      //Serial.println(receivedBytes[i]);
		}
    /**
    * Si on recoit des données conformes
    */
    if(size == 4) {
      state = STATE_PRET;
      for(int i=0;i<4;i++){
        ordreTouches[i] = receivedBytes[i];
      }
    }
    /**
		 ** Si lancement du jeu
    */
    if(size == 1 && receivedBytes[0] == 0 && state == STATE_PRET) {
      state = STATE_RAS;
      digitalWrite(LED, LOW);
      return;
    }
  }
  
}

int boutonAppuye(){
  if(digitalRead(BOUTON1) == 0){
    return 1;
  } else if(digitalRead(BOUTON2) == 0){
    return 2;
  } else if(digitalRead(BOUTON3) == 0){
    return 3;
  } else if(digitalRead(BOUTON4) == 0){
    return 4;
  }
  return 0;
}