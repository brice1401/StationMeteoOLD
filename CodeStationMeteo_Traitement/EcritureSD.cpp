#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
#include "Arduino.h"

RTC_DS1307 rtc;


int getHeure() {
  DateTime now = rtc.now();
   return now.hour();
}

int getMinute() {
  DateTime now = rtc.now();
   return now.minute();
}

int getSeconde() {
  DateTime now = rtc.now();
   return now.second();
}

int getJour() {
  DateTime now = rtc.now();
   return now.day();
}

int getMois() {
  DateTime now = rtc.now();
   return now.month();
}

int getAnnee() {
  DateTime now = rtc.now();
   return now.year();
}

String getDate() {
  DateTime now = rtc.now();
  int Annee = now.year();
  int Mois = now.month();
  int Jour = now.day();
  String Date = String(Jour) + '/' + String(Mois) + '/' + String(Annee);
  return(Date);
}

String getDateJM() {
  DateTime now = rtc.now();
  int Mois = now.month();
  int Jour = now.day();
  String Date = String(Jour) + '/' + String(Mois);
  return(Date);
}

String getHoraire(){
  DateTime now = rtc.now();
  int Heure = now.hour();
  int Minute = now.minute();
  int Seconde = now.second();
  String Horaire = String(Heure) + ":" + String(Minute) + ":" + String(Seconde);

  return(Horaire);
}

String getHoraireHM(){
  DateTime now = rtc.now();
  int Heure = now.hour();
  int Minute = now.minute();
  String Horaire = String(Heure) + ":" + String(Minute);

  return(Horaire);
}

float Moyenne(float Liste[], int Length){
  // Calcul la moyenne de la liste
  int sum;
  for (int i=0; i < Length; i++){
    sum = sum + Liste[i];
  }
  return(sum/Length);
}

float Somme(float Liste[], int Length){
  // Calcul la somme de la liste
  int sum;
  for (int i=0; i < Length; i++){
    sum = sum + Liste[i];
  }
  return(sum);
}


void DecodeReception(char Message[], char DataTraitee[], int LengthData){
  //Fonction qui decode le message recu
  String ChaineCarac;
  
  //Conversion des bytes en charactere
  for (byte i = 0; i < LengthData; i++)
    {
      ChaineCarac += (char)Message[i];
    }

  //on parcourt la chaine de caracrtere pour sortir les données de mesure
  int iData = 0;
  for (int j = 4; j < LengthData; j++){  //commence à 4 pour eviter le probleme des lettres au début
    if(ChaineCarac[j]<65){
      iData += 1;
    }
    else {
      DataTraitee[iData] += ChaineCarac[j];
    }
  }
}
