
// Some code reused from GPS test example for Adafruit Ultimate GPS
// http://www.adafruit.com/products/746

#include "transforms.h"
#include <Adafruit_GPS.h>
#include <Arduino.h>
#include <FlexCAN.h>
#include "SdFat.h"



#define mySerial Serial1

Adafruit_GPS GPS(&mySerial);
FlexCAN fc = FlexCAN(500000);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences.
#define GPSECHO true

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

uint32_t timer = millis();

boolean setOrigin = true;
float distance = 0;
float val;
mypair<float> pair(val, val);
mypair<float> origin(val, val);

CAN_filter_t pos_accel_in;
CAN_filter_t vel_in;

transforms tf;

String str;

SdFatSdio sd;
File file;


void gps_loop() {
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    // Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived()
    // flag to false

    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag
                                    // to false
      return; // we can fail to parse a sentence in which case we should just
              // wait for another
  }

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())
    timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  // Main part to edit for GPS Configureations
  if (millis() - timer > 1000) {
    timer = millis(); // reset the timer

    Serial.print("Fix: ");
    Serial.println((int)GPS.fix);
    if (GPS.fix) {
      file.write("Location: ");
      str = String(GPS.latitudeDegrees, 8);
      file.write(str.c_str());
      str = String((GPS.longitudeDegrees * -1), 8);
      file.write(str.c_str());

      Serial.print("Satellites: ");
      Serial.println((int)GPS.satellites);

      file.write("Satellites: ");
      str = String((int)GPS.satellites, 10);
      file.write(str.c_str());

      if (setOrigin) {
        tf.setPhi_naught(GPS.latitudeDegrees);
        pair = tf.equirectangular((GPS.longitudeDegrees * -1),
                                  GPS.latitudeDegrees);
        origin = pair;
        setOrigin = false;
      } else {
        pair = tf.equirectangular((GPS.longitudeDegrees * -1),
                                  GPS.latitudeDegrees);
        distance = tf.euclidean(origin, pair);
      }


      
      file.write("Point x = ");
      str = String(pair.getFirst(), 8);
      file.write(str.c_str());

      file.write("Point y = ");
      str = String(pair.getSecond(), 8);
      file.write(str.c_str());

      file.write("Origin x = ");
      str = String(origin.getFirst(), 8);
      file.write(str.c_str());

      file.write("Origin y = ");
      str = String(origin.getSecond(), 8);
      file.write(str.c_str());

      file.write("Distance = ");
      str = String(distance, 8);
      file.write(str.c_str());
      
      file.write("\n");

      file.sync();
    }
  }
}


void setup() {


  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  // connect at 115200 so we can read the GPS fast enough and echo without
  // dropping chars also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic test!");
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  mySerial.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data)
  // including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  // GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data,
  // and print it out we don't suggest using anything higher than 1 Hz
  delay(1000);
  // Ask for firmware version
  mySerial.println(PMTK_Q_RELEASE);


  if (!sd.begin()) {
    Serial.println("SdFatSdio begin() failed");
  }

  if (!file.open("testresults4.txt", O_RDWR | O_CREAT)) {
    Serial.println("file open failed");
  }

  delay(20);


}

void loop() {
  GPS.read();

  gps_loop();
  // canbus_loop();
}
