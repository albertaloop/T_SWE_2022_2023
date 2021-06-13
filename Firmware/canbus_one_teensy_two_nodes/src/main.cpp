#include <FlexCAN_T4.h>
#include <StensTimer.h>

#include "node_stubs.hpp"
#include "utils.hpp"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can2;

enum CAN_ID {
  CAN_BMS_ID = 0x100,
  CAN_NAV_ID = 0x200,
  CAN_BRK_ID = 0x300,
  CAN_MTR_ID = 0x400,
  CAN_OBC_ID = 0x500,
};

CAN_message_t bms_msg;
CAN_message_t obc_msg;
struct bms _bms;
StensTimer* bmsTimer = NULL;
const int TICK_ACTION = 1;

void prepare_bms_message(struct bms &_bms, CAN_message_t &msg) {
  int_to_bytes2(_bms.current, msg.buf);
  int_to_bytes2(_bms.voltage, &msg.buf[4]);
}

void timerCallback(Timer* timer){
  if(TICK_ACTION == timer->getAction()){
    update_bms(_bms);
  }
}

void setup(void) {
  can1.begin();
  can2.begin();
  can1.setBaudRate(250000);
  can2.setBaudRate(250000);
  can1.onReceive(canSniff);
  can2.onReceive(canSniff);

  seed_random(analogRead(0));
  bms_msg.id = CAN_BMS_ID;
  bms_msg.len = 8;
  obc_msg.id = CAN_OBC_ID;
  obc_msg.len = 4;

  bmsTimer = StensTimer::getInstance();
  bmsTimer->setStaticCallback(timerCallback);
  bmsTimer->setInterval(TICK_ACTION, 1000); // Timer* myFirstTimer =
}

void print_buffer(const CAN_message_t &msg) {
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
}

void canSniff(const CAN_message_t &msg) {
  // This is NOT working
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  print_buffer(msg);
}

int gcount = 0;
CAN_message_t gmsg;
char a[100];
void loop() {
  bmsTimer->run();
  if (gcount++ > 10000) {
    prepare_bms_message(_bms, bms_msg);
    can1.write(bms_msg);
    can2.write(obc_msg);
    sprintf(a, "writing to CANBUS %x: %d, CANBUS %x: %d", bms_msg.id, bms_msg.len, obc_msg.id, obc_msg.len);
    Serial.println(a);
    Serial.print("TX: "); print_buffer(bms_msg);
    Serial.print("TX: "); print_buffer(obc_msg);
    gcount = 0;
  }
  if (can1.read(gmsg)) {
    Serial.print("RX: "); print_buffer(gmsg);
  }
  if (can2.read(gmsg)) {
    Serial.print("RX: "); print_buffer(gmsg);
  }
}
