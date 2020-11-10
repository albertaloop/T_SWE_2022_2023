#ifndef __CANBus__
#define __CANBus__

#include <FlexCAN.h>
#include "circular_buffer.h"

class CANBus
{
public:
    CANBus(FlexCAN* fc);
    FlexCAN* fc;
    CircularBuffer<CAN_message_t, 10> incoming_buffer;
    CircularBuffer<CAN_message_t, 10> outgoing_buffer;
    void read_msg();
    void send_msg();
    void read_incoming();
    void send_outgoing();
};



#endif