// Prototypes
void ds18B_setup(void);
void do_max6675_loop(void);
void do_ds18B_loop(void);

// Defines
#define ThermoOnly 1  // For use with IDE Tools --> Serial Plotter

#include <Wire.h>


// Sample Arduino MAX6675 Arduino Sketch

#include "max6675.h"

int ktcSO = 0;
int ktcCS = 31;
int ktcCLK = 32;

// TMP36 variables
int sensePin = A9;
int sensorInput;
double temp;

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

void setup(void)
{
  // start serial port
  Serial.begin(9600);

#ifndef ThermoOnly
  Serial.println("Dallas Temperature IC Control Library Demo");
  Serial.println("...and MAX6675 Thermocouple.");
#endif

  // give the MAX a little time to settle
  delay(500);
}

void loop(void) {

#ifndef ThermoOnly
   Serial.println("\n-START-");

   Serial.println("Thermocouple Temp.");
   do_max6675_loop();

   Serial.print("-END-");
   Serial.print("\n");
#else
//  do_max6675_loop(); // Only o/p thermocouple data.
  Serial.print("MAX6675 ");
  double max_temp = ktc.readCelsius();
  Serial.print(max_temp);
  Serial.print(" C, ");
#endif

  // TMP36 Serial monitor output
  sensorInput = analogRead(sensePin);
  temp = (double)sensorInput / 1024;
  temp *= 5;
  temp -= 0.5;
  temp *= 100;

  double temp_to_C = temp - 273.15;

  Serial.print("TMP36temp ");
  Serial.print(temp_to_C);
  Serial.print(" C, ");

  Serial.print("Diff ");
  Serial.println(max_temp - temp_to_C);


  delay(500);
}
