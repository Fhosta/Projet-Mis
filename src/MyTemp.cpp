/*
 * MyTemp.cpp
 *
 *  Created on: 11.05.2022
 *      Author: Florian Hostachy
 */
#include "MyTemp.h"

//Lire la température du dht 22 sur le port 0 pour ensuite l'afficher 
MyTemp::MyTemp(uint8_t dhtPin,uint8_t dhtType){
    dht = new DHT(dhtPin,dhtType);
    dht->begin();
}



float MyTemp::readTemperature()
{
    float temp =  dht-> readTemperature();
    return (temp);
}