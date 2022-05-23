#include <Arduino.h>
#include <utility/imumaths.h>
#include <SparkFunLSM9DS1.h>
#include <SPI.h>
#include "estimator.h"
#include "CANBus.h"
#include <Adafruit_GPS.h>


#define ISR_INTERVAL 1000000 // 100,000 microseconds = 50 ms
#define total_responses 4

// Pin definitions for the accelerometer (LSM9DS1)
#define LSM9DS1_M_CS 37  
#define LSM9DS1_AG_CS	36

bool isAccel_connected = false; // Flag for detecting the accelerometer

LSM9DS1 lsm; // Uses the LSM9DS1 class to create an object 

FlexCAN fc = FlexCAN();
estimator est = estimator();
CANBus can = CANBus(&fc, 0);
imu::Vector<3> accel;
IntervalTimer interruptTimer;

volatile bool interrupt_flag = false;

enum { state0 = 0, state1, state2, state3 };

int state = 0;
CAN_message_t msg_out;
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
  lsm.readAccel();
  
  est.x[2] = lsm.calcAccel(lsm.ax) * 9.81; // Normalize the accelerometer values to m/s^2

  // iterate prediction
  est.predict();
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

 // Checing the lsm connections
 if(lsm.beginSPI(LSM9DS1_AG_CS, LSM9DS1_M_CS) == false)
    Serial.println("Failed to communicate with LSM9DS1!");
   
  else {
    Serial.println("LSM9DS1 Connection Successful");
    isAccel_connected = true;
  }
  delay(1000);

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
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
        break;
      case (state2) : // wait
        break;
      case (state3) : // 100 ms loop
        measurement_loop();
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

