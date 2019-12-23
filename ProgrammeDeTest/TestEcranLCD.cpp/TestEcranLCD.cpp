#include "Arduino.h"
#include <LiquidCrystal.h>


//Pin pour l'�cran LCD
const int PinRS = 2;
const int PinEnable = 3;
const int PinD4 = 4;
const int PinD5 = 5;
const int PinD6 = 6;
const int PinD7 = 7;

// Variables et pins pour l'affichage
const int PinChangeEcran = A2;
const int PinButtonReset = A3;

bool Affichage;
String MessageLCD0;
String MessageLCD1;

int PositionReset;
int PositionChange;
int LastPositionReset;
int LastPositionChange;

int NumEcran;

//Temps d'affichage des valeurs sur l'�cran
const int TimeAffichageMax = 5*1000;
int TimeAffichageCourant = 0;
int debutAffichage = 0;
unsigned long LastdebounceTime = 0;  // the last time the output pin was toggled
unsigned int debounceDelay = 200;    // the debounce time; increase if the output flickers

//Controle du fond
int PinLedFond = A1;


//Initilisation de l'ecran LCD
LiquidCrystal lcd(PinRS, PinEnable, PinD4, PinD5, PinD6, PinD7);

int PositionButton(int EntreeAnalog){
  // si bouton appuie : 1
  // si bouton relache : 0
  //On fait un changement de boutton que lorsque l'on passe de 1 � 0

  int PositionButton;

  if(EntreeAnalog > 500){
    PositionButton = 1;
  }
  else{
    PositionButton = 0;
  }
  return(PositionButton);
}

void setup() {

  Serial.begin(9600);

  pinMode (PinLedFond, OUTPUT);

  Affichage = false; //parametre d'affichage
  NumEcran = 0;

  lcd.begin(16, 2);

  //Initilisation des position des boutons pour l'affichage
  PositionReset = PositionButton(analogRead(PinButtonReset));
  PositionChange = PositionButton(analogRead(PinChangeEcran));
  LastPositionReset = PositionReset;
  LastPositionChange = PositionChange;


}

float DataAffichage[4];
void SelectionDonneeAffichage(int numero, float DataAffichage[]){
  //Fonction qui modifie les valeurs qui sont envoy�es � l'�cran LCD

  if(numero == 0){
    //on affiche le nombre de mm tombe
    float VolumeEau = DataAffichage[numero];
    MessageLCD0 = "Pluie :    |";
    int PartieEnt = int(VolumeEau);
    int PremiereDeci = (int(PartieEnt*10)-10*PartieEnt);
    MessageLCD1 = String(PartieEnt) + "," + String(PremiereDeci);
  }
  else if(numero == 1){
    //Affiche les donn�es sur la direction du vent
    MessageLCD0 = "Direction vent";

    int IndiceVent = DataAffichage[numero];
    String DirectionVentAffichage = "Nord Ouest";
    MessageLCD1 = DirectionVentAffichage;
  }
  else if(numero == 2){
    //Affiche les donn�es sur la force du vent
    MessageLCD0 = "Force vent-km/h";

    float VitesseVentAffichage = DataAffichage[numero]*0.24;
    MessageLCD1 = String(VitesseVentAffichage);
    }
  else if(numero == 3){
    //Affiche les donn�es sur la temp�rature
    MessageLCD0 = "Pas de prise en";
    MessageLCD1 = "compte de la T�";
    }
}


void loop(){

  //Pour g�rer l'affichage
  //On regarde si il y a eu un changement d'�tat du bouton change
  PositionChange = PositionButton(analogRead(PinChangeEcran));
  if(PositionChange != LastPositionChange){
    LastdebounceTime = millis();
    }
  if((millis() - LastdebounceTime) > debounceDelay){
    if((LastPositionChange == LOW) && (PositionChange != LastPositionChange)){
      NumEcran = (NumEcran + 1) % 4;
      Affichage = true;
      debutAffichage = millis(); //Reinitialise le timer d'affichage
      LastPositionChange = PositionChange;
    }

  }
  TimeAffichageCourant = millis() - debutAffichage;
  if(TimeAffichageCourant < TimeAffichageMax){
    //regarde depuis combien de temps c'est affiche, on coupe si trop longptemps
    Affichage = true;
  }
  else{
    Affichage = false;
  }


  //Pour g�rer le reset des donn�es
  //On regarde si il y a eu un changement d'�tat du bouton reset
  PositionReset = PositionButton(analogRead(PinButtonReset));
  if(PositionReset != LastPositionReset){
    LastdebounceTime = millis();
  }
  if((millis() - LastdebounceTime) > debounceDelay){
    if((PositionReset != LastPositionReset) && (LastPositionReset = LOW)){
      Serial.print("tous est remis � z�ro \n");
    }
  }

  if(Affichage){
    //Si on affiche les donn�es ou pas
    //On va chercher les donnees pour remplir le vecteur DataAffichage

    DataAffichage[0] = 42;
    DataAffichage[1] = 26;
    DataAffichage[2] = 30;
    DataAffichage[3] = 5;

    //Initialisation des lignes � afficher
    SelectionDonneeAffichage(NumEcran, DataAffichage);

    lcd.display();
    lcd.setCursor(0, 0);
    lcd.print("                "); //efface les caract�re en memoire
    lcd.setCursor(0, 0);
    lcd.print(MessageLCD0);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print(MessageLCD1);
    analogWrite (PinLedFond, 255);
    analogWrite (PinLedFond, 128);

    }
    else{
      lcd.noDisplay();
      analogWrite (PinLedFond, 0);
    }

}
