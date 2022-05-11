/* Copyright (C) 2022 Alain Dube
 * All rights reserved.
 *
 * Projet Stone de base
 * Ecole du Web
 * Cours Systèmes embarqués (c)2022
 *
    @file     main.cpp
    @author   Alain Dubé
    @version  1.1 22/08/15
    @description
      Démonstration comment utiliser le PORT SERIE pour accèder aux fonctionnalités
      de l'écran STONE en utilisant la classe MyStone et MySerial

    platform = ESP32
    OS : Arduino
    Langage : C++

    Historique des versions
        Version    Date       Auteur       Description
        1.1        22/08/15  Alain       Première version du logiciel

    Fonctionnalités implantées
        Lecture des evénements envoyés par l'écran
        Envoyer une commande à l'écran
          Optenir la version du Firmware de l'écran
 * */

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define RXD2 18//16
#define TXD2 19//17
#define BAUD_RATE 115200

#include <iostream>

#include "MyButton.h"
MyButton *myButtonT4 = new MyButton();
MyButton *myButtonT5 = new MyButton();

#include "MyStone.h"
MyStone *myStone;

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

  bool boutonStoneStart;
  float tempBois = 25.0;
  int compteur = 0; 
  int compteurBois = 20;


//Thread qui permet de LOOPER et lire le contenu du port serie
//avec l'aide du la fonction getValidsDatasIfExists
void readStoneData() {
  datasRead rd = myStone->getValidsDatasIfExists();
  //std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
  switch(rd.id){

      case 0x0002: { //Version
          std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
          std::string stoneVersion = rd.name;
          std::cout << "Version : " <<  stoneVersion.c_str() << "\n";

          //std::this_thread::sleep_for(std::chrono::milliseconds(10));

          break;
          }

          case 0x1001: { //Bouton de démarrage
          std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
          std::string stoneStart = rd.name;
          std::cout << "Bouton de démarrage : " <<  stoneStart.c_str() << "\n";
          if(strcmp(stoneStart.c_str(),"start")== 0 && (rd.type == 2))
          boutonStoneStart = true;

          //std::this_thread::sleep_for(std::chrono::milliseconds(10));

          break;
          }
      }

  if(rd.id<0) std::cout << "Data received ( id: : " << intToHexa(abs(rd.id)) << "  Command: " << rd.command << " Type: " << rd.type<< ")\n";
}
#define DHTPIN 0     
#define DHTTYPE    DHT22    
#include "MyTemp.h"
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
 

   readStoneData();
    float temp = myTemp.readTemperature();
      if(isnan(temp)){
        Serial.println(F("Failed to read temp"));
        return;
        }

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
  boutonStoneStart = 0;
  compteur = 0;
  char strCuisson[64];
  delay(600);
  sprintf(strCuisson, "%d /20s", compteur);
  myStone ->setLabel("cuisson",strCuisson);
  myStone ->setLabel("avertissement","");
  delay(100);
}