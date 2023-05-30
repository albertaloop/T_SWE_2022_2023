#include "CANBus.h"
#include "estimator.h"
#include <Adafruit_BNO055.h>
#include <Adafruit_GPS.h>
#include <Arduino.h>
#include <SPI.h>
#include <utility/imumaths.h>

#define ISR_INTERVAL 1000000 // 100,000 microseconds = 50 ms
#define total_responses 255

FlexCAN fc = FlexCAN(500000);
estimator est = estimator();
Adafruit_BNO055 bno = Adafruit_BNO055(55);
imu::Vector<3> accel;
IntervalTimer interruptTimer;

#define mySerial Serial1

Adafruit_GPS GPS(&mySerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences.
#define GPSECHO true

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

// uint32_t timer = millis();

volatile bool interrupt_flag = false;
bool new_z = false;

enum { state0 = 0, state1, state2, state3 };
int state = 0;
float z = 0;

CAN_message_t pos_accel_out;
CAN_message_t vel_out;
CAN_message_t recenter_ack_out;
CAN_filter_t mask = {(uint8_t)0x00, (uint8_t)0x00, (uint32_t)0x04fffff};

typedef union {
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;

void (*responses[total_responses])(CAN_message_t msg_in);

void recenterCallback(CAN_message_t msg_in) {
  for (int i = 0; i < 3; i++) {
    est.x[i] = 0;
  }
}

void positionCallback(CAN_message_t msg_in) {
  Serial.println("position call back");
  FLOATUNION_t t;
  memcpy(t.bytes, msg_in.buf, 4);
  z = t.number;
}

void timerCallback(void) { interrupt_flag = true; }

void measurement_loop(void) {
  GPS.read();
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
  // if (timer > millis())  timer = millis();

  // // approximately every 2 seconds or so, print out the current stats
  // // Main part to edit for GPS Configureations
  // if (millis() - timer > 2000) {
  //   timer = millis(); // reset the timer

  Serial.print("Fix: ");
  Serial.println((int)GPS.fix);
  if (GPS.fix) {
    Serial.print("Location: ");
    Serial.print(GPS.latitudeDegrees, 6);
    Serial.print(GPS.lat);
    Serial.print(", ");
    Serial.print((GPS.longitudeDegrees * -1), 6);
    Serial.println(GPS.lon);

    Serial.print("Satellites: ");
    Serial.println((int)GPS.satellites);
  }
  // iterate prediction
  //   est.predict();
  // if (new_z) {
  //   est.update(z);
  // }
  // }
}

void canbus_loop(void) {
  // receive message
  CAN_message_t msg_in;
  msg_in.timeout = 1;
  if (fc.read(msg_in)) {
    // respond to message
    Serial.println(msg_in.id);
    responses[msg_in.id >> 24](msg_in);
  }
  // Send telemetry
  FLOATUNION_t t;
  memcpy(t.bytes, est.x, 4);
  for (int i = 0; i < 4; i++) {
    pos_accel_out.buf[i] = t.bytes[i];
  }
  memcpy(t.bytes, est.x + 2, 4);
  for (int i = 0; i < 4; i++) {
    pos_accel_out.buf[i + 4] = t.bytes[i];
  }
  fc.write(pos_accel_out);
  memcpy(t.bytes, est.x + 1, 4);
  for (int i = 0; i < 4; i++) {
    vel_out.buf[i] = t.bytes[i];
  }
}

void setup() {

  responses[4] = positionCallback;
  responses[15] = recenterCallback;

  pos_accel_out.ext = 0;
  pos_accel_out.id = 0x01ffffff;
  pos_accel_out.len = 8;
  memset(pos_accel_out.buf, 0, 8);

  vel_out.ext = 0;
  vel_out.id = 0x03ffffff;
  vel_out.len = 4;
  memset(vel_out.buf, 0, 8);

  recenter_ack_out.ext = 0;
  recenter_ack_out.id = 0x02ffffff;
  recenter_ack_out.len = 1;
  memset(vel_out.buf, 0, 8);

  /* Initialize accelerometer */
  if (!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    while (1)
      ;
  }
  delay(1000);
  bno.setExtCrystalUse(true);

  /* Initialize GPS */
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  // connect at 115200 so we can read the GPS fast enough and echo without
  // dropping chars also spit it out connect at 115200 so we can read the GPS
  // fast enough and echo without dropping chars also spit it out
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
  interruptTimer.begin(timerCallback, ISR_INTERVAL);
  fc.begin(mask);
}

void loop() {
  if (interrupt_flag) {
    switch (state) {
    case (state0): { // 200 ms loop
      // canbus_loop();
      // Serial.println("state0");
      break;
    }
    case (state1): { // 100 ms loop
      measurement_loop();
      Serial.println("state1");
      break;
    }
    case (state2): { // wait
      // Serial.println("state2");
      break;
    }
    case (state3): { // 100 ms loop
      measurement_loop();
      // Serial.println("state3");
      break;
    }
    }
    if (state < 3) {
      state = state + 1;
    } else {
      state = 0;
    }
    interrupt_flag = false;
  }
}
