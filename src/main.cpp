/* Copyright (C) 2022 Alain Dube
 * All rights reserved.
 *
 * Projet Stone de base
 * Ecole du Web
 * Cours Systèmes embarqués (c)2022
 *
    @file     main.cpp
    @author   Alain Dubé
    @colabor  Florian Hostachy
    @version  1.1 11/05/22

    @description
      Projet qui permet l'affichage de la température d'un four, pour pouvoir lancer la cuisson d'un bois
      sélectionner quand le four est à bonne température un minuteur se lancera pour le temps cuisson.

    platform = ESP32
    OS : Arduino
    Langage : C++

    Historique des versions
        Version    Date       Auteur       Description
        1.0        22/08/15  Alain       Première version du logiciel
        1.1        11/05/22  Florian     Version fonctionnel du logiciel 

    Fonctionnalités implantées
        Lecture des evénements envoyés par l'écran
        Envoyer une commande à l'écran
        Actionner un bouton pour démarrer le four quand il est a la bonne température
        Un compteur est présent pour le temps de cuisson

 * */


//ajout des librairies nécessaire au bon fonctionnement
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//Définition des différents parametres de fonctionnement et des entrée 
#define RXD2 18//16
#define TXD2 19//17
#define BAUD_RATE 115200
#define DHTPIN 0     
#define DHTTYPE    DHT22    

#include <iostream>

// ajout des class nécessaire au bon fonctionnement
#include "MyButton.h"
#include "MyTemp.h"
#include "MyStone.h"

//Pointeur mystone
MyStone *myStone;

//Définition du bouton d'activation et  du système de compteur
bool boutonStoneStart;
  float tempBois = 25.0;
  int compteur = 0; 
  int compteurBois = 20;

std::string intToHexa(int value){
  char buffer[10];
  sprintf(buffer , "0x%4X", value);
  return (buffer);
};

std::string charToString(char str, std::string formatStr){
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), str);
  return (buffer);
};

std::string intToString(int value, std::string formatStr){
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), value);
  return (buffer);
};

//Thread qui permet de LOOPER et lire le contenu du port serie
//avec l'aide du la fonction getValidsDatasIfExists
void readStoneData() {
  datasRead rd = myStone->getValidsDatasIfExists();

  switch(rd.id){

      case 0x0002: { //Version
          std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
          std::string stoneVersion = rd.name;
          std::cout << "Version : " <<  stoneVersion.c_str() << "\n";
          break;
          }

          case 0x1001: { //Bouton de démarrage
          std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
          std::string stoneStart = rd.name;
          std::cout << "Bouton de démarrage : " <<  stoneStart.c_str() << "\n";
          if(strcmp(stoneStart.c_str(),"start")== 0 && (rd.type == 2))
          boutonStoneStart = true;
          break;
          }
      }

  if(rd.id<0) std::cout << "Data received ( id: : " << intToHexa(abs(rd.id)) << "  Command: " << rd.command << " Type: " << rd.type<< ")\n";
}

//Pointeur myTemp
MyTemp myTemp(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(9600);

  Serial.println("Stone serial Txd is on pin: "+String(TXD2));
  Serial.println("Stone serial Rxd is on pin: "+String(RXD2));
  myStone = new MyStone(115200, SERIAL_8N1, RXD2, TXD2);

  cout << std::string("Début de l'exemple Stone de base pour le ESP32")  << "\n";
}
void loop() 
{ 
 
//Si la température n'arrive pas à être lu on aura un message d'erreur
   readStoneData();
    float temp = myTemp.readTemperature();
      if(isnan(temp)){
        Serial.println(F("Erreur de température vérifier les branchement"));
        return;
        }

  //Moyen pour afficher la température
  char strTemperature[64];
  sprintf(strTemperature, "%g Celcius", temp);
  myStone->setLabel("temp", strTemperature);

  //Gestion du temps de cuissson
  if(boutonStoneStart == 1)
  {
    if(temp >= tempBois)
    {
      while (compteur < compteurBois)
      {
        compteur++;
        Serial.println(compteur);
        char strCuisson[64];
        sprintf(strCuisson, "%d /20s", compteur);
        myStone ->setLabel("cuisson",strCuisson);
        myStone ->setLabel("avertissement","Cuisson en cours...");
        if(compteur == compteurBois)
        {
          myStone ->setLabel("avertissement","Cuisson du four terminé");
        }
        delay(1000);
      }
    }
  }

  //Remise du bouton sur la position zéro pour ne pas retourner dans la boucle
  boutonStoneStart = 0;

  // Compteur remis à zéro
  compteur = 0;

  //Affichage divers pour la cuisson
  char strCuisson[64];
  delay(600);
  sprintf(strCuisson, "%d /20s", compteur);
  myStone ->setLabel("cuisson",strCuisson);
  myStone ->setLabel("avertissement","");
  delay(100);
}