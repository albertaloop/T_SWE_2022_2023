
//For tests change the sensor variable to responding number
//Max6675 is 1
//Tmp36 is 2
const int sensor = 1;

#include <Wire.h>
const int min_room_temp = 10;
const int max_room_temp = 30;

// Sample Arduino MAX6675 Arduino Sketch

if(sensor == 1){
  #include "max6675.h"
  int ktcSO = 0;
  int ktcCS = 31;
  int ktcCLK = 32;
  MAX6675 ktc(ktcCLK, ktcCS, ktcSO);
}else if (sensor == 2){
  int sensePin = A9;
  int sensorInput;
  double temp;
}

void setup(void)
{
  Serial.begin(9600);
}

//Integration tests
void loop(void) {
  //room temp test
    if(test_room_temp()){Serial.println("Room temp test passed");}
    delay(500);
}

//Roomtemp test returns true if test is data fetched is with roomtemp, false otherwise
bool test_room_temp(void){
  //reads for 10 data points
  if (sensor == 1){
    for (size_t i = 0; i < 10; i++){
      if(!(min_room_temp < ktc.readCelsius() < max_room_temp)){
        Serial.println("Test Room Temperature failed!");
        return false;
      }
    }
  }else if(sensor == 2){
    sensorInput = analogRead(sensePin);
    temp = (double)sensorInput / 1024;
    temp = (((temp*5)-0.5)*100); //TODO: check if this is calculated in C
    for (size_t i = 0; i < 10; i++){
      if(!(min_room_temp < temp < max_room_temp)){
        Serial.println("Test Room Temperature failed!");
        return false;
      }
    }
  }
  //true if roomtemp test passed
  return true;
}