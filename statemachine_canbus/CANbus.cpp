#include "CANBus.h"
#include "circular_buffer.h"

CANBus::CANBus(){
    // the constructor
}
CANBus::~CANBus(){
    // do something or leave it empty
}

void CANBus::write_buffer(CAN_message_t msg){
    this->flexcan.write(&msg);
}

void CANBus::send_msg(){
    CAN_message_t msg_send;
    this->flexcan.read(&msg_send);
    this->buffer1.Write(msg_send);
}

void CANBus::read_buffer(){
    // I'm not sure what should read_buffer do.
    
}

void CANBus::read_msg(){
    // read from incoming buffer
    CAN_message_t msg_read = buffer2.Read();
    // write to FlexCAN object
    this->flexcan.write(&msg_read);
}