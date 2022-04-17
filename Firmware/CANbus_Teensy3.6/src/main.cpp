#include "messages.h"
#include <Arduino.h>
#include <CANBus.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>

#define FLEXCANb_MCR(b) (*(vuint32_t *)(b))
CAN_filter_t mask = {0, 0, 0};
CAN_message_t message1;
CAN_message_t message2;
CAN_message_t message3;
extern FlexCAN Can0;
FlexCAN Can0(50000);
CANBus canbus = CANBus(&Can0, &mask);

void setup() {
  // put your setup code here, to run once:
  mask.id = 0xBFF;
  mask.ext = 0;
  mask.rtr = 0;

  message1.ext = 0;
  message1.id = 0xFFF;
  message1.len = 4;
  message1.timeout = 0;
  message1.buf[0] = 0b01010101;

  message2.ext = 0;
  message2.id = 0xffffffff;
  message2.len = 1;
  message2.timeout = 0;
  message2.buf[0] = 0b00000000;

  Serial.begin(9600);
  // canbus.fc->begin(mask);
  // delay(20);
  // Serial.println("SETUP");
  // Serial.print("MCR = ");
  // Serial.println(FLEXCAN0_MCR, BIN);
}

void loop() {

  Serial.print("Sent msg: ");
  Serial.println(message1.buf[0]);
  canbus.send_msg(message1);
  // Serial.print("MCR = ");
  // Serial.println(FLEXCANb_MCR(FLEXCAN0_BASE), BIN);
  // Serial.print("CTRL1 = ");
  // Serial.println(FLEXCAN0_CTRL1, BIN);

  // CAN_message_t msg_out = canbus.outgoing_buffer.Read();

  canbus.broadcast();
  delay(2);
  canbus.receive();
  message3 = canbus.read_msg();
  Serial.print("Received msg: ");
  Serial.println(message3.buf[0], BIN);

  // Serial.print("Message 1 buf = ");
  // for (int i = 0; i < 7; i++) {
  //   Serial.print(message1.buf[i]);
  //   Serial.print(", ");
  // }
  // Serial.println(message1.buf[7]);

  // Serial.print("Message 3 buf =");
  //   for (int i = 0; i < 7; i++) {
  //   Serial.print(message3.buf[i]);
  //   Serial.print(", ");
  // }
  // Serial.println(message3.buf[7]);
  // canbus.send_msg(message2);
  // message3 = canbus.read_msg();

  // put your main code here, to run repeatedly:
}
