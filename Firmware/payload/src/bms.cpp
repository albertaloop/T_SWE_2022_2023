#include <FlexCAN_T4.h>

#include "utils.hpp"

void prepare_bms_message(struct bms &_bms, CAN_message_t &msg) {
  int_to_bytes(msg.buf, _bms.current);
  int_to_bytes(&msg.buf[4], _bms.voltage);
}