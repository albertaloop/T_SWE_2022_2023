#ifndef _MSG_H_
#define _MSG_H_

#include <Arduino.h>

/*
  Contains CANbus messages.
*/
class MessageMaster {
  public:
    void buildSndMessages(uint8_t *snd_msg_list);
    void buildRcvMessages(uint8_t *rcv_msg_list);
};

#endif