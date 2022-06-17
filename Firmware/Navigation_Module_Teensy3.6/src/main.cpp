#include <Arduino.h>
#include <utility/imumaths.h>
#include <Adafruit_BNO055.h>
#include <SPI.h>
#include "estimator.h"
#include "CANBus.h"
#include <Adafruit_GPS.h>


#define ISR_INTERVAL 1000000 // 100,000 microseconds = 50 ms
#define total_responses 255

FlexCAN fc = FlexCAN(500000);
estimator est = estimator();
Adafruit_BNO055 bno = Adafruit_BNO055(55);
imu::Vector<3> accel;
IntervalTimer interruptTimer;

volatile bool interrupt_flag = false;
bool new_z = false;

enum { state0 = 0, state1, state2, state3 };
int state = 0;
float z = 0;

CAN_message_t pos_accel_out;
CAN_message_t vel_out;
CAN_message_t recenter_ack_out;
CAN_filter_t mask = {(uint8_t)0x00, (uint8_t)0x00, (uint32_t)0x04fffff};
Adafruit_GPS GPS(&Serial1);
boolean usingInterrupt = false;
void useInterrupt(boolean);

typedef union
{
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;

void (*responses[total_responses])( CAN_message_t msg_in );

void recenterCallback(CAN_message_t msg_in) {
  for(int i = 0; i < 3; i++) {
    est.x[i] = 0;
  }
}

void positionCallback(CAN_message_t msg_in) {
    Serial.println("position call back");
    FLOATUNION_t t;
    memcpy(t.bytes, msg_in.buf, 4);
    z = t.number;
}

void timerCallback(void) {
  interrupt_flag = true;
}

void measurement_loop(void) {
  // iterate prediction
    est.predict();
  if (new_z) {
    est.update(z);
  }
}

void canbus_loop(void) {
  // receive message
  CAN_message_t msg_in;
  msg_in.timeout = 1;
  if(fc.read(msg_in)) {
    // respond to message
    Serial.println(msg_in.id);
    responses[msg_in.id >> 24](msg_in);
  }
  // Send telemetry
  FLOATUNION_t t;
  memcpy(t.bytes, est.x, 4);
  for(int i = 0; i < 4; i++) {
    pos_accel_out.buf[i] = t.bytes[i];
  }
  memcpy(t.bytes, est.x + 2, 4);
  for(int i = 0; i < 4; i++) {
    pos_accel_out.buf[i+4] = t.bytes[i];
  }
  fc.write(pos_accel_out);
  memcpy(t.bytes, est.x + 1, 4);
  for(int i = 0; i < 4; i++) {
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
  Serial.begin(9600);

  interruptTimer.begin(timerCallback, ISR_INTERVAL);
  fc.begin(mask);
}

void loop() {
  if(interrupt_flag) {
    switch (state) {
      case (state0) : {// 200 ms loop
        canbus_loop();
        Serial.println("state0");
        break;
      }
      case (state1) : {// 100 ms loop
        measurement_loop();
          Serial.println("state1");
        break;
      }
      case (state2) : {// wait
        Serial.println("state2");
        break;
      }
      case (state3) : {// 100 ms loop
        measurement_loop();
        Serial.println("state3");
        break;
      }   
    }
    if(state < 3) {
      state = state + 1;
    } else {
      state = 0;
    }
    interrupt_flag = false;
  }   
}

