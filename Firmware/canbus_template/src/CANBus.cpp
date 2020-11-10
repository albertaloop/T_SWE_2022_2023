#include "CANBus.h"
#include "circular_buffer.h"

CANBus::CANBus(FlexCAN* fc) {
    fc = fc;
}

void CANBus::read_msg() {
    // take message out of incoming buffer and process per message ID
}

void CANBus::send_msg() {
    // send CAN_message_t to outgoing buffer
}

void CANBus::read_incoming() {
    // use read() function to pass message into incoming buffer
}

void CANBus::send_outgoing() {
    // use write() funciton to empty outgoing buffer
}