#include "CANBus.h"
#include "circular_buffer.h"
#include <FlexCAN.h>
#include <kinetis_flexcan.h>

CANBus::CANBus(FlexCAN *fc, CAN_filter_t *mask) {
  this->fc = fc;
  if (mask == 0) {
    this->fc->begin();
  } else {
    this->fc->begin(*mask);
  }
}

CAN_message_t CANBus::read_msg() {
  // Read CAN_message_t from incoming buffer
  CAN_message_t msg_in = this->incoming_buffer.Read();
  return msg_in;
}

void CANBus::send_msg(CAN_message_t msg_out) {
  // Write CAN_message_t to outgoing buffer
  // Serial.println(msg_out.buf[0]);
  this->outgoing_buffer.Write(msg_out);
}

void CANBus::receive() {
  // use read() function to pass message into incoming buffer
  CAN_message_t msg_in;
  this->fc->read(msg_in);
  this->incoming_buffer.Write(msg_in);
}

void CANBus::broadcast() {
  // use write() function to empty outgoing buffer
  CAN_message_t msg_out = this->outgoing_buffer.Read();
  // message read from buffer
  // Serial.println(msg_out.buf[0]);
  this->fc->write(msg_out);
}
