#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <FlexCAN_T4.h>

static CAN_message_t msg;
FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> Can0;

void canSniff(const CAN_message_t &msg)
{
  Serial.print("MB ");
  Serial.print(msg.mb);
  Serial.print("  OVERRUN: ");
  Serial.print(msg.flags.overrun);
  Serial.print("  LEN: ");
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
}

void setup()
{
  // put your setup code here, to run once:
  Wire.begin();
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);
  for (size_t i = 0; i < 10; i++)
  {
    digitalWrite(14, HIGH);
    delay(100);
    digitalWrite(14, LOW);
  }

  digitalWrite(14, LOW);

  Serial.begin(9600);
  Serial.println("Serial Connected \n");

  Can0.begin();
  Can0.setBaudRate(500000);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.onReceive(FIFO, canSniff);
  Can0.mailboxStatus();

  //  delay(1000);
}

void loop()
{
  Can0.events();
  
  msg.len = 0x3;
  msg.id = 0x1;

  Can0.read(msg);
  delay(100);
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
  for ( uint8_t i = 0; i < msg.len; i++ ) {
      Serial.print(msg.buf[i], HEX);
      Serial.print(" ");
  }
  Serial.println("\n");

  if ((msg.buf[0] + msg.buf[1] + msg.buf[2])/100 > 0)
  {
    digitalWrite(14, HIGH);
    delay(250);
  }
  digitalWrite(14, LOW);
}