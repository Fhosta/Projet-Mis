/* Copyright (C) 2022 Florian Hostachy
 * All rights reserved.
 *
 * Projet Domotique
 * Ecole du Web
* Cours Systèmes embarqués (c)2022
 *  
    @file     MyTemp.h
    @author   Florian Hostachy
    @version  1.1 11/05/22 
    @description
      Permet l'utilisation du dht22

    platform = ESP32
    OS = Arduino
    Langage : C++


    Historique des versions
        Version    Date       Auteur       Description
        1.1        11/05/22  Florian       Première version du logiciel

**/
#ifndef MYTEMP_H
#define MYTEMP_H

#include <Arduino.h>
#include <string>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

class MyTemp
{
    private :
        DHT *dht; 
    public :
        MyTemp(uint8_t dhtPin, uint8_t dhtType);
        ~MyTemp() { };
        void init();
        float readTemperature();
        
};
#endif
