/*
 * MySerial.cpp
 *
 *  Created on: 01.11.2022
 *      Author: Alain
 */
#include "MyTemp.h"

MyTemp::MyTemp(uint8_t dhtPin,uint8_t dhtType){
    dht = new DHT(dhtPin,dhtType);
    dht->begin();
}



float MyTemp::readTemperature()
{
    float temp =  dht-> readTemperature();
    return (temp);
}