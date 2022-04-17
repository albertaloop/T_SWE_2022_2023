// Prototypes
void ds18B_setup(void);
void do_max6675_loop(void);
void do_ds18B_loop(void);
bool test_room_temp(void);

// Defines
#define ThermoOnly 1 // For use with IDE Tools --> Serial Plotter

#include <Wire.h>

// Sample Arduino MAX6675 Arduino Sketch
#include "max6675.h"
const int min_room_temp = 10;
const int max_room_temp = 30;

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

void setup(void) {
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
  // room temp test
  if (test_room_temp()) {
    Serial.println("Room temp test passed");
  }
  delay(500);
}

// Roomtemp test returns true if test is data fetched is with roomtemp, false
// otherwise
bool test_room_temp(void) {
  // reads for 10 data points
  for (size_t i = 0; i < 10; i++) {
    if (!(min_room_temp < ktc.readCelsius() < max_room_temp)) {
      Serial.println("Test Room Temperature failed!");
      return false;
    }
  }
  // true if roomtemp test passed
  delay(500);
}
