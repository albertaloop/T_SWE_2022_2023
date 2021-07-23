#include "CANBus.h"
#include "circular_buffer.h"
#include <FlexCAN.h>

CANBus::CANBus(FlexCAN *fc, CAN_filter_t *mask)
{
    this->fc = fc;
    fc->begin(*mask);
}

CAN_message_t CANBus::read_msg()
{
    // Read CAN_message_t from incoming buffer
    CAN_message_t msg_in = this->incoming_buffer.Read();
    return msg_in;
}

void CANBus::send_msg(CAN_message_t msg_out)
{
    // Write CAN_message_t to outgoing buffer
    this->outgoing_buffer.Write(msg_out);
}

void CANBus::receive()
{
    // use read() function to pass message into incoming buffer
    CAN_message_t msg_in;
    this->fc->read(msg_in);
    this->incoming_buffer.Write(msg_in);
}

void CANBus::broadcast()
{
    // use write() function to empty outgoing buffer
    CAN_message_t msg_out = this->outgoing_buffer.Read();
    this->fc->write(msg_out);
}