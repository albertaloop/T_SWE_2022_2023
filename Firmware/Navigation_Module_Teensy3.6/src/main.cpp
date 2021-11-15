#include <Arduino.h>
#include <utility/imumaths.h>
#include <Adafruit_BNO055.h>
#include <SPI.h>
#include "estimator.h"
#include "CANBus.h"

#define ISR_INTERVAL 50000 // 100,000 microseconds = 50 ms
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

struct {
  float x[3];
} state_params;

void (*responses[2])( void );


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
  accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  // set accel state parameter to measurement
  est.x_bar[2] = (float)accel.x();
  // iterate prediction
  est.predict();
  // update state params
  for (int i = 0; i < 3; i++) {
    state_params.x[i] = est.x_bar[i];
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
    est.x_bar[i] = castIntToFloat(msg_out.buf[i]);
  }
}


void position_request() {
  for (int i = 0; i < 3; i++) {
    msg_out.buf[0] = castFloatToInt(est.x_bar[0]);
  }
  can.fc->write(msg_out);
}


void velocity_request() {
  for (int i = 0; i < 3; i++) {
    msg_out.buf[0] = castFloatToInt(est.x_bar[1]);
  }
  can.fc->write(msg_out);
}


void acceleration_request() {
  for (int i = 0; i < 3; i++) {
    msg_out.buf[0] = castFloatToInt(est.x_bar[2]);
  }
  can.fc->write(msg_out);
}

void setup() {
  responses[0] = recenter;
  responses[1] = position_request;
  responses[2] = velocity_request;
  responses[0] = acceleration_request;

  msg_out.ext = 0;
  msg_out.id = 0;
  msg_out.len = 1;
  memset(msg_out.buf, 0, 8);
  /* Initialise the sensor */
  if( !bno.begin() )
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    while(1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);
  interruptTimer.begin(timerCallback, ISR_INTERVAL);
}

void loop() {
  if(interrupt_flag) {
    switch (state) {
      case (state0) : // 200 ms loop
        canbus_loop();
        break;
      case (state1) : // 100 ms loop
        measurement_loop();
        break;
      case (state2) : // wait
        break;
      case (state3) : // 100 ms loop
        canbus_loop();
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

