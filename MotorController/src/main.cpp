#include <Arduino.h>
#include "MessageMaster.h"
#include <Wire.h>
#include <SPI.h>
#include <FlexCAN_T4.h>

FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> Can0;

MessageMaster messageCreator;
uint8_t rcv_msg_list[3][5];
uint8_t snd_msg_list[9][4];
int ind;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  Serial.println("Serial Begin! \n");

  messageCreator.buildRcvMessages(rcv_msg_list);
  messageCreator.buildSndMessages(snd_msg_list);

  Can0.begin();
  Can0.setBaudRate(500000);
  Can0.setMaxMB(16);

  ind = 0;
}

void loop() {
  CAN_message_t msg;
  msg.id = 0x1;
  msg.len = snd_msg_list[ind][0];
  msg.buf[0] = snd_msg_list[ind][1];
  msg.buf[1] = snd_msg_list[ind][2];
  msg.buf[2] = snd_msg_list[ind][3];
  Can0.write(msg);

  Serial.print("MB ");
  Serial.print(msg.mb);
  Serial.print(" OVERRUN: ");
  Serial.print(msg.flags.overrun);
  Serial.print(" LEN: ");
  Serial.print(msg.len);
  Serial.print(" EXT: ");
  Serial.print(msg.flags.extended);
  Serial.print(" TS: ");
  Serial.print(msg.timestamp);
  Serial.print(" ID: ");
  Serial.print(msg.id, HEX);
  Serial.print(" Buffer: ");
  for (uint8_t i = 0; i < msg.len; i++)
  {
    Serial.print(msg.buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println("\n");

  ind++;
  if (ind==9) {
    ind = 0;
  }
  delay(1000);
}