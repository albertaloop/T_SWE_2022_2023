#include "cactus_io_BME280_SPI.h"
#include <SPI.h>

#define BME_SCK 32  // Serial Clock
#define BME_MISO 12 // Serial Data Out
#define BME_MOSI 11 // Serial Data In
#define BME_CS 31   // Chip Select

// Create BME280 object
// BME280_SPI bme(BME_CS); // Using Hardware SPI
BME280_SPI bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // Using Software SPI

void setup() {

  Serial.begin(9600);
  Serial.println("Bosch BME280 Pressure - Humidity - Temp Sensor | cactus.io");

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }

  bme.setTempCal(-1); // Sensor was reading high so offset by 1 degree C

  Serial.println("Pressure\tHumdity\t\tTemp\ttTemp");
}

void loop() {

  bme.readSensor();
  double pressure = bme.getPressure_MB();
  Serial.print(pressure + 106.85);
  Serial.print(" mb\t"); // Pressure in millibars
  Serial.print(bme.getHumidity());
  Serial.print(" %\t\t");
  Serial.print(bme.getTemperature_C());
  Serial.print(" *C\t");
  Serial.print(bme.getTemperature_F());
  Serial.println(" *F");

  // Add a 2 second delay.
  delay(2000); // just here to slow down the output.
}
