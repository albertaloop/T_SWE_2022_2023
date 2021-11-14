#include <Arduino.h>

/*
  Contains CANbus messages.
*/
class MessageMaster {
  public:
    uint8_t *buildSndMessages(uint8_t *snd_msg_list);
    uint8_t *buildRcvMessages(uint8_t *rcv_msg_list);
};