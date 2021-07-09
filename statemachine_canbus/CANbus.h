#ifndef _CANBUS_H_
#define _CANBUS_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <utility>
#include <cassert>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "FlexCAN.h"
#include "circular_buffer.h"

class CANBus
{
public:
    CANBus();
    ~CANBus();

    void write_buffer(CAN_message_t msg);
    void send_msg();

    void read_buffer();
    void read_msg();

private:
    FlexCAN flexcan;
    // outgoing
    CircularBuffer<CAN_message_t, 16> buffer1;
    // incoming
    CircularBuffer<CAN_message_t, 16> buffer2;
}

#endif