#include <Arduino.h>
#include <utility/imumaths.h>
#include <Adafruit_BNO055.h>
#include <SPI.h>
#include "estimator.h"
#include "CANBus.h"
#include <Adafruit_GPS.h>


#define ISR_INTERVAL 1000000 // 100,000 microseconds = 50 ms
#define total_responses 4

FlexCAN fc = FlexCAN();
estimator est = estimator();
CANBus can = CANBus(&fc, 0);
Adafruit_BNO055 bno = Adafruit_BNO055(55);
imu::Vector<3> accel;
IntervalTimer interruptTimer;

volatile bool interrupt_flag = false;

enum { state0 = 0, state1, state2, state3 };

int state = 0;
CAN_message_t msg_out;
Adafruit_GPS GPS(&Serial1);
boolean usingInterrupt = false;
void useInterrupt(boolean);

struct {
  float x[3];
} state_params;

void (*responses[total_responses])( void );


void timerCallback(void) {
  interrupt_flag = true;
}

int castFloatToInt(float f) {
    union { float f; int i; } u;
    u.f = f;
    return u.i;
}


float castIntToFloat(int i) {
    union { float f; int i; } u;
    u.i = i;
    return u.f;
}


void measurement_loop(void) {
  // get accel measurement
  // accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  // set accel state parameter to measurement
  // est.x[2] = (float)accel.x();


    // if a sentence is received, we can check the checksum, parse it...
      noInterrupts();
          char c = GPS.read();
      if (c) Serial.print(c);
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA()));   // this also sets the newNMEAreceived() flag to false
      // return;  // we can fail to parse a sentence in which case we should just wait for another
      Serial.print(GPS.fix);

  }
    interrupts();
  // iterate prediction
  est.predict();

  if (GPS.fix) {
    Serial.print("Location: ");
    Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
    Serial.print(", "); 
    Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
  }
  // update state params
  for (int i = 0; i < 3; i++) {
    state_params.x[i] = est.x[i];
  }
}


void canbus_loop(void) {
  // receive message
  CAN_message_t msg_in;
  if(can.fc->read(msg_in)) {
    uint8_t msg_num = msg_in.buf[0];
    // respond to message
    responses[msg_num % total_responses]();
  }
}

void recenter() {
  for (int i = 0; i < 3; i++) {
    est.x[i] = castIntToFloat(msg_out.buf[i]);
  }
}

void position_request() {
  for (int i = 0; i < 3; i++) {
    msg_out.buf[0] = castFloatToInt(est.x[0]);
  }
  can.fc->write(msg_out);
}

void velocity_request() {
  for (int i = 0; i < 3; i++) {
    msg_out.buf[0] = castFloatToInt(est.x[1]);
  }
  can.fc->write(msg_out);
}

void acceleration_request() {
  for (int i = 0; i < 3; i++) {
    msg_out.buf[0] = castFloatToInt(est.x[2]);
  }
  can.fc->write(msg_out);
}

// float latLonToPostion(float lat, float lon){
  
// }


void setup() {
  responses[0] = recenter;
  responses[1] = position_request;
  responses[2] = velocity_request;
  responses[3] = acceleration_request;

  msg_out.ext = 0;
  msg_out.id = 0;
  msg_out.len = 1;
  memset(msg_out.buf, 0, 8);
  /* Initialize accelerometer */
  if( !bno.begin() )
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    while(1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);

  /* Initialize GPS */
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic test!");
    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  Serial1.begin(9600);
    // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
    delay(1000);
  interruptTimer.begin(timerCallback, ISR_INTERVAL);


}

void loop() {
  if(interrupt_flag) {
    switch (state) {
      case (state0) : // 200 ms loop
        // canbus_loop();

        break;
      case (state1) : // 100 ms loop
        measurement_loop();
        Serial.print(GPS.fix);
        break;
      case (state2) : // wait
        break;
      case (state3) : // 100 ms loop
        accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
        est.x[2] = (float)accel.x();

        break;   
    }
    if(state < state3) {
      state = state + 1;
    } else {
      state = 0;
    }
    interrupt_flag = false;
  }   
}

