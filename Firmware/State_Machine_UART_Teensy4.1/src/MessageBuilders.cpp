#include "MessageBuilders.h"

uint8_t*  build_packet(uint8_t team_id, uint8_t status, int32_t acceleration, int32_t position, int32_t velocity) {
  static uint8_t packet[PACKET_LENGTH];
  packet[0] = team_id;  
  packet[1] = status;

  for (int i = ACCELERATION_IDX + 3; i >= ACCELERATION_IDX; i--) {
    packet[i] = acceleration & 0xFF;
    acceleration = acceleration >> 8;
  }

  for (int i = POSITION_IDX + 3; i >= POSITION_IDX; i--) {
    packet[i] = position & 0xFF;
    position = position >> 8;
  }

  for (int i = VELOCITY_IDX + 3; i >= VELOCITY_IDX; i--) {
    packet[i] = velocity & 0xFF;
    velocity = velocity >> 8;
  }

  for (int i = BATTERY_VOLTAGE_IDX; i < PACKET_LENGTH; i++) {
    packet[i] = 0;
  }

  return packet;
}

void init_CAN_messages() {
  CAN_messages.nav_recenter.id = (uint32_t)0x0fffffff;
  CAN_messages.nav_recenter.len = (uint8_t)0x01;

  CAN_messages.state_estop.id = (uint32_t)0x00ffffff;
  CAN_messages.state_estop.len = (uint8_t)0x01;

  CAN_messages.state_prepare_launch.id = (uint32_t)0x0effffff;
  CAN_messages.state_prepare_launch.len = (uint8_t)0x01;

  CAN_messages.state_begin_launch.id = (uint32_t)0x0dffffff;
  CAN_messages.state_begin_launch.len = (uint8_t)0x01;
  
  CAN_messages.state_soft_stop.id = (uint32_t)0x0cffffff;
  CAN_messages.state_soft_stop.len = (uint8_t)0x01;

  CAN_messages.state_shut_down.id = (uint32_t)0x0bffffff;
  CAN_messages.state_shut_down.len = (uint8_t)0x01;

}