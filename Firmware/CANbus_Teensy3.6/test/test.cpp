#include <Arduino.h>
#include <FlexCAN.h>
#include <unity.h>

using namespace std;

void setup() {
  // put your setup code here, to run once:
  CAN_filter_t mask;
  mask.id = 0xBFF;
  mask.ext = 0;
  mask.rtr - 0;
  CANbus canbus = CANBus(*mask);

  CAN_Message_t message1;
  message1.ext = 0;
  message1.id = (uint32_t)mask;
  message1.len = 1;
  message1.timeout = 0;
  message1.buf[0] = 0b01010101;

  CAN_Message_t message2;
  message2.ext = 0;
  message2.id = 0xffffffff;
  message2.len = 1;
  message2.timeout = 0;
  message2.buf[0] = 0b00000000;
}

void loop() {}
