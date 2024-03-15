/**
* Module Morse
  Données reçues : aucune
  Exemple :
*/

#include <Arduino.h>
#include <Wire.h>
#include "TM1637Display.h"

#define GAUCHE 9
#define MILIEU 10
#define DROITE 11
#define LED 8
#define LED_MORSE 5

#define CLC 6
#define DIO 7

#define MODULE_ADDR 3
#define BUFFER_SIZE 4

#define STATE_RAS 0
#define STATE_DESARME 1 
#define STATE_ERREUR 2
#define STATE_ATTENTE_CONFIGURATION 3
#define STATE_PRET 4

#define MAX_LONGUEUR 14

#define NB_SOLUTIONS 15 // de 0 à 15 soit 16

char correspondance_morse[][MAX_LONGUEUR] = {
  "_ ___", //A
  "___ _ _ _", //B
  "___ _ ___ _", //C
  "___ _ _", //D
  "_", //E
  "_ _ ___ _", //F
  "___ ___ _", //G
  "_ _ _ _", //H
  "_ _", //I
  "_ ___ ___ ___", //J
  "___ _ ___", //K
  "_ ___ _ _", //L
  "___ ___", //M
  "___ _", //N
  "___ ___ ___", //O
  "_ ___ ___ _", //P
  "___ ___ _ ___", //Q
  "_ ___ _", //R
  "_ _ _", //S
  "___", //T
  "_ _ ___", //U
  "_ _ _ ___", //V
  "_ ___ ___", //W
  "___ _ _ ___", //X
  "___ _ ___ ___", //Y
  "___ ___ _ _" //Z
};
char mot[][6] = {
  "vitre",
  "ville",
  "chose",
  "signe",
  "linge",
  "ligne",
  "champ",
  "litre",
  "phase",
  "chaud",
  "bille",
  "balle",
  "singe",
  "plume",
  "pluie",
  "salle"
};
unsigned int frequence[]{
  3505,
  3515,
  3522,
  3532,
  3535,
  3542,
  3545,
  3552,
  3555,
  3565,
  3572,
  3575,
  3582,
  3592,
  3595,
  3600
};

char state;
char appui;
char indice_actuel;
unsigned char indice_solution;
byte receivedBytes[BUFFER_SIZE];

TM1637Display display(CLC, DIO); // CLC, DIO
char* correspondance_frequence(char* mot);
void afficher7segments(char indice);
void requestEvent();
void receiveEvent(int);
int boutonAppuye();
void maj_frequence();

void setup() {
  // put your setup code here, to run once:
  display.setBrightness(4); // luminosité de 0 à 7
  display.clear();

  //I2C
  Wire.begin(MODULE_ADDR);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  Serial.begin(9600);

  //les 3 touches
  pinMode(DROITE,INPUT_PULLUP);
  pinMode(MILIEU,INPUT_PULLUP);
  pinMode(GAUCHE,INPUT_PULLUP);

  //la LED
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);

  //LED MORSE
  pinMode(LED_MORSE,OUTPUT);
  digitalWrite(LED_MORSE,LOW);
  //FIN LED MORSE
  
  //l'état
  state = STATE_PRET;

  //l'indice aléatoire pour la solution
  randomSeed(analogRead(0));
  indice_solution = random(16);

  Serial.println(indice_solution);

  //l'indice actuel
  indice_actuel = 0;

  //on affiche 0000 avant de commencer la partie
  display.showNumberDec(0000, true, 4, 0); 

  Serial.print("Indice : ");
  Serial.println(indice_solution);
  Serial.print("Mot : ");
  Serial.println(mot[indice_solution]);
}

/*
* un '_' : 1 seconde allumé
* un ' ' : 1 seconde éteint
* entre deux mots : 7 secondes
* Les signaux longs ont été remplacés par 3 courts, car un long dure 3 secondes
*/
void loop() {
  // put your main code here, to run repeatedly:
  if(state == STATE_RAS){
    //Ici affichage du mot en morse;
    char lettre_actuelle;
    char* morse_actuel;
    unsigned long temps_debut;
    unsigned long temps_fin;
    maj_frequence();
    


    for(int i=0; i<6; i++){
      lettre_actuelle = mot[indice_solution][i]; //on récupère le mot d'indice solution, lettre par lettre
      morse_actuel = correspondance_morse[lettre_actuelle-'a']; //on récupère la fréquence de la lettre actuelle
      for(int j=0; morse_actuel[j] != '\0'; j++){ //on boucle sur les caractères du code morse
        if(morse_actuel[j] == '_'){ //Si c'est un _, on allume la LED pendant une seconde
          temps_debut = millis();
          temps_fin = millis();
          digitalWrite(LED_MORSE,HIGH);
          while(temps_fin - temps_debut < 1000){
            maj_frequence();
            temps_fin = millis();
          }
        } else { //sinon c'est un ' ', donc on éteint la LED pendant 1 seconde
          temps_debut = millis();
          temps_fin = millis();
          digitalWrite(LED_MORSE,LOW);
          while(temps_fin - temps_debut < 1000){
            maj_frequence();
            temps_fin = millis();
          }
        }
      } //On a fini la lettre -> pause de 3 secondes
      temps_debut = millis();
      temps_fin = millis();
      digitalWrite(LED_MORSE,LOW);
      while(temps_fin - temps_debut < 3000){
        maj_frequence();
        temps_fin = millis();
      }
    } //On a fini le mot -> pause de 7 secondes
    temps_debut = millis();
    temps_fin = millis();
    digitalWrite(LED_MORSE,LOW);
    while(temps_fin - temps_debut < 7000){
      maj_frequence();
      temps_fin = millis();
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
		 ** Si lancement du jeu
    */
    if(size == 1 && receivedBytes[0] == 0 && state == STATE_PRET) {
      state = STATE_RAS;
      digitalWrite(LED, LOW);
      afficher7segments(indice_actuel);
      return;
    }
  }
  
}

int boutonAppuye(){
  if(digitalRead(GAUCHE) == 0){
    return 1;
  } else if(digitalRead(MILIEU) == 0){
    return 2;
  } else if(digitalRead(DROITE) == 0){
    return 3;
  }
  return 0;
}

void afficher7segments(char indice){
  unsigned int maFrequence = frequence[indice];
  display.showNumberDec(maFrequence, true, 4, 0); 
}

//NE PAS RESTER APPUYE TROP LONGTEMPS
void maj_frequence(){
  if((appui = boutonAppuye()) != 0){ //si appui sur un bouton
    Serial.print("Appui touche : ");
    Serial.println(appui);
    switch(appui){
      case 1 : //On a appuyé à gauche
        //on décale l'indice à gauche
        if(indice_actuel == 0) indice_actuel = NB_SOLUTIONS;
        else indice_actuel--;
        afficher7segments(indice_actuel);
        break;

      case 2 : //On a appuyé au milieu
        if(indice_actuel == indice_solution) {
          digitalWrite(LED,HIGH);
          state = STATE_DESARME;
        } else {
          state = STATE_ERREUR;
        }
        break;

      case 3 : //On a appuyé à droite
        //on décale l'indice à droite
        if(indice_actuel == NB_SOLUTIONS) indice_actuel = 0;
        else indice_actuel++;
        afficher7segments(indice_actuel);
        break;
    }
    while(boutonAppuye() != 0); //on attend que l'utilisateur relâche le bouton
    delay(100);
  }
}