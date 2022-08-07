#ifndef __CANBus__
#define __CANBus__

#include "circular_buffer.h"
#include <FlexCAN.h>

class CANBus {
public:
  CANBus(FlexCAN *fc, CAN_filter_t *mask);
  CANBus(FlexCAN *fc);
  FlexCAN *fc;
  CircularBuffer<CAN_message_t, 10> incoming_buffer;
  CircularBuffer<CAN_message_t, 10> outgoing_buffer;
  CAN_message_t read_msg();
  void send_msg(CAN_message_t msg_out);
  void receive();
  void broadcast();
};

#endif
