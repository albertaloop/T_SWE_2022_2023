#include <SPI.h>
#include "cactus_io_BME280_SPI.h"

#define BME_SCK 32// Serial Clock
#define BME_MISO 12// Serial Data Out
#define BME_MOSI 11// Serial Data In
#define BME_CS 31// Chip Select

// Create BME280 object
//BME280_SPI bme(BME_CS); // Using Hardware SPI
BME280_SPI bme(BME_CS,BME_MOSI,BME_MISO,BME_SCK); // Using Software SPI
const int min_room_temp = 10;
const int max_room_temp = 30;
//Summer: 45% - 55%, Winter:25% - 35%
const int min_humitity = 30;
const int max_humitity = 50;
//Edmonton daily average pressure ranges 1013 - 1031 mbar
const int min_pressure = 1005;
const int max_pressure = 1035;


void setup() {

Serial.begin(9600);
if (!bme.begin()) {
Serial.println("Could not find a valid BME280 sensor, check wiring!");
while (1);
}

bme.setTempCal(-1);// Sensor was reading high so offset by 1 degree C
}

void loop() {

bme.readSensor();

//reset vars
bool tempTest = false;
bool pressureTest = false;
bool humidityTest =false;

tempTest = test_room_temp();
pressureTest = test_avg_pressure();
humidityTest = test_avg_humitity();

if( tempTest && pressureTest && humidityTest){
    Serial.println("All tests passed!!");
}

// Add a 2 second delay.
delay(2000); //just here to slow down the output.
}

bool test_room_temp(void){
  //reads for 10 data points
    for (size_t i = 0; i < 10; i++){
        if(!(min_room_temp < bme.getTemperature_C() < max_room_temp)){
        Serial.println("Test Room Temperature failed!");
        return false;
        }
    }
  //true if roomtemp test passed
  return true;
}

bool test_avg_humitity(void){
  //reads for 10 data points
    for (size_t i = 0; i < 10; i++){
        if(!(min_humitity < bme.getHumidity() < max_humitity)){
        Serial.println("Test humidity failed!");
        return false;
        }
    }
  return true;
}

bool test_avg_pressure(void){
  //reads for 10 data points
    for (size_t i = 0; i < 10; i++){
        double pressure = bme.getPressure_MB() + 106.85; //in milibar
        if(!(min_pressure< pressure < max_pressure)){
        Serial.println("Test pressure failed!");
        return false;
        }
    }
  return true;
}