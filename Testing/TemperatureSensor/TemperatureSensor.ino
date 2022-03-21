// Prototypes
void ds18B_setup(void);
void do_max6675_loop(void);
void do_ds18B_loop(void);

// Defines
#define ThermoOnly 1  // For use with IDE Tools --> Serial Plotter
#include <Wire.h>

const int min_room_temp = 10;
const int max_room_temp = 30;
// Sample Arduino MAX6675 Arduino Sketch

#include "max6675.h"

int ktcSO = 0;
int ktcCS = 31;
int ktcCLK = 32;

MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

void do_max6675_loop(void) {
  // basic readout test

#ifndef ThermoOnly
   Serial.print("Temp = ");
   Serial.print(ktc.readCelsius());
   Serial.print("\t Deg F = ");
   Serial.print(ktc.readFahrenheit());
   Serial.print(" Deg C");
#else
   Serial.println(ktc.readCelsius());
#endif
}


//#include <OneWire.h>
//#include <DallasTemperature.h>
//
//// Data wire is plugged into port 2 on the Arduino
//#define ONE_WIRE_BUS 4
//
//// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
//OneWire oneWire(ONE_WIRE_BUS);
//
//// Pass our oneWire reference to Dallas Temperature.
//DallasTemperature DallasSensors(&oneWire);
//
void setup(void)
{
  // start serial port
  Serial.begin(9600);

#ifndef ThermoOnly
  Serial.println("Dallas Temperature IC Control Library Demo");
  Serial.println("...and MAX6675 Thermocouple.");

//  // Start up the library
//  DallasSensors.begin();
#endif

  // give the MAX a little time to settle
  delay(500);
}

//void do_ds18B_loop(void)
//{
//  // call DallasSensors.requestTemperatures() to issue a global temperature
//  // request to all devices on the bus
//  Serial.print("Requesting 1-wire devices...");
//  DallasSensors.requestTemperatures(); // Send the command to get temperatures
//  Serial.println("DONE");
//
//  Serial.print("Temperature for the device 1 (index 0) is: ");
//  Serial.print(DallasSensors.getTempCByIndex(0));
//  Serial.println(" Deg C");
//}


void loop(void) {
  if(test_room_temp()){Serial.println("Room temp test passed");}
  delay(500);
}

bool test_room_temp(void){
  for (size_t i = 0; i < 10; i++){
    if(!(min_room_temp < ktc.readCelsius() < max_room_temp)){
      Serial.println("Test Room Temperature failed!");
      return false;
    }
  }
  return true;
}