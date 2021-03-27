#include <Arduino.h>
#include <CANBus.h>
#include <FlexCAN.h>
#include "messages.h"

  CAN_filter_t mask;
  CAN_message_t message1;  
  CAN_message_t message2;
  CAN_message_t message3;
  extern FlexCAN Can0;



void setup() {
  // put your setup code here, to run once:
  mask.id = 0xBFF;
  mask.ext = 0;
  mask.rtr = 0;

  message1.ext = 0;
  message1.id = 0xFFF;
  message1.len = 1;
  message1.timeout = 0;
  message1.buf[0] = 0b01010101;
  
  message2.ext = 0;
  message2.id = 0xffffffff;
  message2.len = 1;
  message2.timeout = 0;
  message2.buf[0] = 0b00000000;

  Serial.begin(9600);
}

void loop() {
  FlexCAN Can0(0);
  CANBus canbus = CANBus(&Can0, &mask);
  canbus.send_msg(message1);
  message3 = canbus.read_msg();
  bool pass = (message3.buf[0] == message1.buf[0]);
  Serial.println(pass);

  canbus.send_msg(message2);
  message3 = canbus.read_msg();

  // put your main code here, to run repeatedly:
}